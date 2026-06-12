#include "ui_dashboard.h"
#include "../media/ui_media_tabs.h"
#include <string.h>

GtkWidget *lbl_model = NULL, *lbl_os = NULL, *lbl_battery = NULL, *lbl_serial = NULL, *btn_serial_toggle = NULL;
GtkWidget *lbl_storage_text = NULL, *progress_storage = NULL, *grid_data = NULL, *box_storage = NULL;
GtkWidget *lbl_cat_system = NULL, *lbl_cat_apps = NULL, *lbl_cat_media = NULL;

/* Global handle for the media tab container and its registry backend */
GtkWidget *media_tab_vbox = NULL;
MediaRegistry global_media_registry;

char raw_serial_backup[64] = {0};
bool is_serial_masked = true;

static void on_serial_toggle_clicked(GtkButton *btn, gpointer data) {
    (void)btn; (void)data;
    is_serial_masked = !is_serial_masked;
    refresh_serial_display();
}

GtkWidget *create_device_dashboard(void) {
    GtkWidget *notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

    /* --- PAGE 1: About Device --- */
    GtkWidget *vbox_about = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_start(vbox_about, 25); gtk_widget_set_margin_end(vbox_about, 25);
    gtk_widget_set_margin_top(vbox_about, 25); gtk_widget_set_margin_bottom(vbox_about, 25);

    lbl_model = gtk_label_new("Please connect a mobile device or select an existing one from the sidebar list.");
    gtk_widget_set_halign(lbl_model, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(lbl_model), TRUE);
    gtk_box_append(GTK_BOX(vbox_about), lbl_model);

    grid_data = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid_data), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid_data), 20);
    gtk_widget_set_visible(grid_data, FALSE);
    gtk_box_append(GTK_BOX(vbox_about), grid_data);

    void add_row(int r, const char *k, GtkWidget **v) {
        GtkWidget *kl = gtk_label_new(k); gtk_widget_set_halign(kl, GTK_ALIGN_START);
        *v = gtk_label_new("—"); gtk_widget_set_halign(*v, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(grid_data), kl, 0, r, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_data), *v, 1, r, 1, 1);
    }
    add_row(0, "Operating System:", &lbl_os);
    add_row(1, "Battery Power Status:", &lbl_battery);

    GtkWidget *kl_ser = gtk_label_new("Hardware Serial Key:");
    gtk_widget_set_halign(kl_ser, GTK_ALIGN_START);
    GtkWidget *serial_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    lbl_serial = gtk_label_new("—"); gtk_widget_set_halign(lbl_serial, GTK_ALIGN_START);
    btn_serial_toggle = gtk_button_new_with_label("👁️ Show");
    g_signal_connect(btn_serial_toggle, "clicked", G_CALLBACK(on_serial_toggle_clicked), NULL);
    gtk_box_append(GTK_BOX(serial_hbox), lbl_serial);
    gtk_box_append(GTK_BOX(serial_hbox), btn_serial_toggle);
    gtk_grid_attach(GTK_GRID(grid_data), kl_ser, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_data), serial_hbox, 1, 2, 1, 1);

    box_storage = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(box_storage, 10);
    gtk_widget_set_visible(box_storage, FALSE);
    gtk_box_append(GTK_BOX(vbox_about), box_storage);

    GtkWidget *lbl_st_title = gtk_label_new("Internal Memory Storage Usage:");
    gtk_widget_set_halign(lbl_st_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box_storage), lbl_st_title);

    progress_storage = gtk_progress_bar_new();
    gtk_box_append(GTK_BOX(box_storage), progress_storage);

    void add_cat(GtkWidget **lbl) {
        *lbl = gtk_label_new(""); gtk_widget_set_halign(*lbl, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(box_storage), *lbl);
    }
    add_cat(&lbl_storage_text); add_cat(&lbl_cat_system); add_cat(&lbl_cat_apps); add_cat(&lbl_cat_media);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_about, gtk_label_new("About"));

    /* --- PAGE 2: Advanced Dynamic Media Tab Component --- */
    memset(&global_media_registry, 0, sizeof(MediaRegistry));
    media_tab_vbox = ui_media_tabs_create(&global_media_registry);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), media_tab_vbox, gtk_label_new("Photos & Videos"));

    /* --- PAGES 3, 4: Clean Isolated Tab Layouts --- */
    void add_tab(const char *title, const char *msg) {
        GtkWidget *v = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_widget_set_margin_start(v, 25); gtk_widget_set_margin_top(v, 25);
        GtkWidget *l = gtk_label_new(msg); gtk_widget_set_halign(l, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(v), l);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), v, gtk_label_new(title));
    }
    add_tab("Applications", "📦 Installed user applications catalog ledger pipeline coming soon.");
    add_tab("Files", "📁 Shared system storage shared partition file-tree explorer coming soon.");

    return notebook;
}
