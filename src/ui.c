#include <gtk/gtk.h>
#include <string.h>

int scan_usb_ports(); 
void load_saved_devices(void (*callback)(const char *vid, const char *brand, const char *model, const char *custom_name));
void show_plugins_manager_window(GtkWidget *main_window);
void register_new_device(const char *vid, const char *serial, const char *brand, const char *model, const char *custom_name);
void get_iphone_real_data(char *serial, char *model, char *custom, size_t max_len);
void get_android_real_data(char *serial, char *model, char *custom, size_t max_len);

static GtkWidget *device_list_box = NULL;
static int last_usb_status = 0; 

typedef struct {
    char vendor_id[16];
    GtkWidget *button_widget;
} DeviceWidgetMap;

// Выделяем массив под 20 устройств
static DeviceWidgetMap saved_buttons[20]; 
static int saved_buttons_count = 0;

static void add_device_button_to_ui(const char *vid, const char *brand, const char *model, const char *custom_name) {
    if (saved_buttons_count >= 20) return;

    char full_label[128];
    snprintf(full_label, sizeof(full_label), "%s %s (%s)", brand, model, custom_name);

    GtkWidget *btn = gtk_button_new_with_label(full_label);
    gtk_button_set_has_frame(GTK_BUTTON(btn), FALSE);
    
    GtkWidget *child_label = gtk_button_get_child(GTK_BUTTON(btn));
    if (GTK_IS_LABEL(child_label)) {
        gtk_label_set_xalign(GTK_LABEL(child_label), 0.0);
    }
    gtk_widget_set_margin_start(btn, 10);
    gtk_widget_set_margin_end(btn, 10);
    gtk_widget_set_margin_top(btn, 5);

    gtk_widget_set_sensitive(btn, FALSE);
    gtk_box_append(GTK_BOX(device_list_box), btn);

    strncpy(saved_buttons[saved_buttons_count].vendor_id, vid, 16);
    saved_buttons[saved_buttons_count].button_widget = btn;
    saved_buttons_count++;
}

static void reload_left_panel_devices() {
    // 1. Физически удаляем все кнопки старого списка с экрана
    GtkWidget *child = gtk_widget_get_first_child(device_list_box);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(device_list_box), child);
        child = next;
    }
    
    // ИСПРАВЛЕНО: Обнуляем внутренний счетчик кнопок в памяти программы!
    saved_buttons_count = 0;
    
    // 2. Считываем чистый список из SQLite заново
    load_saved_devices(add_device_button_to_ui);
}

static gboolean on_usb_tick(gpointer user_data) {
    int current_status = scan_usb_ports();

    if (current_status != 0 && last_usb_status == 0) {
        last_usb_status = current_status; // Блокируем повторный вход
        
        char s[64] = {0}, m[64] = {0}, c[64] = {0};
        if (current_status == 1) {
            get_iphone_real_data(s, m, c, 64);
            if (strcmp(s, "UNKNOWN_IOS_SERIAL") != 0 && strstr(m, "Unknown") == NULL && strstr(c, "ERROR") == NULL) {
                register_new_device("05ac", s, "Apple", m, c);
                reload_left_panel_devices();
            } else {
                last_usb_status = 0; // Сброс при ошибке залоченного экрана
            }
        } else if (current_status == 2) {
            get_android_real_data(s, m, c, 64);
            if (strcmp(s, "UNKNOWN_AND_SERIAL") != 0) {
                register_new_device("2717", s, "Xiaomi", m, c);
                reload_left_panel_devices();
            } else {
                last_usb_status = 0;
            }
        }
    } else if (current_status == 0 && last_usb_status != 0) {
        // Кабель выдернули — сбрасываем статус
        last_usb_status = 0;
    }

    // Управление доступностью кнопок (подсветка)
    const char *active_vid = "";
    if (current_status == 1) active_vid = "05ac";
    if (current_status == 2) active_vid = "2717";

    for (int i = 0; i < saved_buttons_count; i++) {
        if (strcmp(saved_buttons[i].vendor_id, active_vid) == 0 || 
            (current_status == 2 && strcmp(saved_buttons[i].vendor_id, "05ac") != 0)) { 
            gtk_widget_set_sensitive(saved_buttons[i].button_widget, TRUE);
        } else {
            gtk_widget_set_sensitive(saved_buttons[i].button_widget, FALSE);
        }
    }
    return TRUE; 
}

static void on_plugins_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    show_plugins_manager_window(main_window);
}

void build_ui(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ProjectPhone");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *header = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    GtkWidget *btn_plugins = gtk_button_new_with_label("🔌 Плагины");
    g_signal_connect(btn_plugins, "clicked", G_CALLBACK(on_plugins_clicked), window);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), btn_plugins);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(left_panel, 250, -1);
    gtk_box_append(GTK_BOX(main_box), left_panel);

    GtkWidget *lbl_title = gtk_label_new("📱 Мои устройства:");
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_widget_set_margin_start(lbl_title, 15);
    gtk_widget_set_margin_top(lbl_title, 15);
    gtk_widget_set_margin_bottom(lbl_title, 5);
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl_title), attrs);
    pango_attr_list_unref(attrs);
    gtk_box_append(GTK_BOX(left_panel), lbl_title);

    device_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(left_panel), device_list_box);

    reload_left_panel_devices();

    GtkWidget *btn_all_new = gtk_button_new_with_label("➕ Новый гаджет");
    gtk_button_set_has_frame(GTK_BUTTON(btn_all_new), FALSE);
    GtkWidget *lbl_new_child = gtk_button_get_child(GTK_BUTTON(btn_all_new));
    if (GTK_IS_LABEL(lbl_new_child)) gtk_label_set_xalign(GTK_LABEL(lbl_new_child), 0.0);
    gtk_widget_set_margin_start(btn_all_new, 10);
    gtk_widget_set_margin_top(btn_all_new, 15);
    gtk_box_append(GTK_BOX(left_panel), btn_all_new);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(main_box), separator);

    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(right_panel, TRUE);
    gtk_box_append(GTK_BOX(main_box), right_panel);

    GtkWidget *lbl_main = gtk_label_new("Пожалуйста, подключите устройство или выберите его в списке слева.");
    gtk_box_append(GTK_BOX(right_panel), lbl_main);

    g_timeout_add(1000, on_usb_tick, NULL);
    gtk_window_present(GTK_WINDOW(window));
}
