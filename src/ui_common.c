#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void refresh_plugins_window();

// Массив общих пакетов и команд для проверки
static const char *com_packages[] = {"sqlite3", "curl"};
static const char *com_commands[] = {"sqlite3", "curl"};
static const char *com_names[] = {
    "  • SQLite3 (Локальная база данных)",
    "  • LibCurl (Сетевой движок и облако)"
};
#define COM_COUNT 2

static int is_common_program_installed(const char *cmd) {
    char check_cmd[256]; // ИСПРАВЛЕНО: Теперь это массив на 256 байт
    snprintf(check_cmd, sizeof(check_cmd), "which %s > /dev/null 2>&1", cmd);
    if (system(check_cmd) == 0) return 1;
    return 0;
}

static void on_common_download_clicked(GtkButton *btn, gpointer user_data) {
    const char *system_cmd = (const char *)user_data;
    char terminal_cmd[4096]; // ИСПРАВЛЕНО: Теперь это массив на 4096 байт
    snprintf(terminal_cmd, sizeof(terminal_cmd),
             "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Установка общего компонента %s...\"; "
             "sudo apt update && sudo apt install -y %s; read'", system_cmd, system_cmd);
    system(terminal_cmd);
    refresh_plugins_window();
}

static void on_common_remove_clicked(GtkButton *btn, gpointer user_data) {
    const char *system_cmd = (const char *)user_data;
    char terminal_cmd[4096]; // ИСПРАВЛЕНО: Теперь это массив на 4096 байт
    snprintf(terminal_cmd, sizeof(terminal_cmd),
             "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Удаление общего компонента %s...\"; "
             "sudo apt purge -y %s; read'", system_cmd, system_cmd);
    system(terminal_cmd);
    refresh_plugins_window();
}

static void on_common_bulk_action(GtkButton *btn, gpointer user_data) {
    int action_type = GPOINTER_TO_INT(user_data); 
    char pkg_list[256] = ""; // ИСПРАВЛЕНО: Задали размер буфера списка
    int target_count = 0;

    for (int i = 0; i < COM_COUNT; i++) {
        int installed = is_common_program_installed(com_commands[i]);
        if (action_type == 1 && !installed) {
            strcat(pkg_list, com_packages[i]);
            strcat(pkg_list, " ");
            target_count++;
        } else if (action_type == 2 && installed) {
            strcat(pkg_list, com_packages[i]);
            strcat(pkg_list, " ");
            target_count++;
        }
    }

    if (target_count == 0) return;

    char terminal_cmd[4096]; // ИСПРАВЛЕНО: Огромный буфер под команду
    if (action_type == 1) {
        snprintf(terminal_cmd, sizeof(terminal_cmd),
                 "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Установка общих пакетов: %s...\"; "
                 "sudo apt update && sudo apt install -y %s; read'", pkg_list, pkg_list);
    } else {
        snprintf(terminal_cmd, sizeof(terminal_cmd),
                 "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Безопасное удаление общих пакетов: %s...\"; "
                 "sudo apt purge -y %s; read'", pkg_list, pkg_list);
    }
    system(terminal_cmd);
    refresh_plugins_window();
}

static void add_common_plugin_row(GtkWidget *container, const char *visible_name, const char *system_cmd) {
    GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_bottom(row_box, 5);

    GtkWidget *lbl_name = gtk_label_new(visible_name);
    gtk_widget_set_halign(lbl_name, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl_name, TRUE);
    gtk_box_append(GTK_BOX(row_box), lbl_name);

    int installed = is_common_program_installed(system_cmd);

    if (installed) {
        GtkWidget *lbl_status = gtk_label_new("✅");
        gtk_box_append(GTK_BOX(row_box), lbl_status);

        GtkWidget *btn_remove = gtk_button_new_with_label("🗑️");
        g_signal_connect(btn_remove, "clicked", G_CALLBACK(on_common_remove_clicked), (gpointer)system_cmd);
        gtk_box_append(GTK_BOX(row_box), btn_remove);
    } else {
        GtkWidget *btn_download = gtk_button_new_with_label("📥 Скачать");
        g_signal_connect(btn_download, "clicked", G_CALLBACK(on_common_download_clicked), (gpointer)system_cmd);
        gtk_box_append(GTK_BOX(row_box), btn_download);

        GtkWidget *lbl_status = gtk_label_new("❌");
        gtk_box_append(GTK_BOX(row_box), lbl_status);
    }

    gtk_box_append(GTK_BOX(container), row_box);
}

void build_ui_common(GtkWidget *container) {
    GtkWidget *child = gtk_widget_get_first_child(container);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(container), child);
        child = next;
    }

    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_bottom(header_box, 10);
    gtk_box_append(GTK_BOX(container), header_box);

    GtkWidget *lbl_common_header = gtk_label_new("⚙️ Общие компоненты системы");
    gtk_widget_set_halign(lbl_common_header, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl_common_header, TRUE);
    PangoAttrList *attrs_common = pango_attr_list_new();
    pango_attr_list_insert(attrs_common, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl_common_header), attrs_common);
    pango_attr_list_unref(attrs_common);
    gtk_box_append(GTK_BOX(header_box), lbl_common_header);

    GtkWidget *btn_all_in = gtk_button_new_with_label("📥 Установить всё");
    g_signal_connect(btn_all_in, "clicked", G_CALLBACK(on_common_bulk_action), GINT_TO_POINTER(1));
    gtk_box_append(GTK_BOX(header_box), btn_all_in);

    GtkWidget *btn_all_out = gtk_button_new_with_label("🗑️ Удалить всё");
    g_signal_connect(btn_all_out, "clicked", G_CALLBACK(on_common_bulk_action), GINT_TO_POINTER(2));
    gtk_box_append(GTK_BOX(header_box), btn_all_out);

    for(int i = 0; i < COM_COUNT; i++) {
        add_common_plugin_row(container, com_names[i], com_commands[i]);
    }
}
