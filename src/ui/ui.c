#include <gtk/gtk.h>
#include "ui.h"

GtkWidget *device_list_box = NULL;

/* Header bar action callback linking the newly refactored dependencies window layout */
static void on_dependencies_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    show_dependencies_manager_window(main_window);
}

/* Main workspace layout builder implementation */
void build_ui(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ProjectPhone");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *header = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    /* Header action button for dependency management popup */
    GtkWidget *btn_deps = gtk_button_new_with_label("📦 Dependencies");
    g_signal_connect(btn_deps, "clicked", G_CALLBACK(on_dependencies_clicked), window);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), btn_deps);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    /* --- Left Panel Layout --- */
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(left_panel, 250, -1);
    gtk_box_append(GTK_BOX(main_box), left_panel);

    GtkWidget *lbl_title = gtk_label_new("📱 My Devices:");
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_widget_set_margin_start(lbl_title, 15);
    gtk_widget_set_margin_top(lbl_title, 15);
    gtk_widget_set_margin_bottom(lbl_title, 5);
    
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl_title), attrs);
    pango_attr_list_unref(attrs);
    gtk_box_append(GTK_BOX(left_panel), lbl_title);

    /* Dynamic device button list container box node */
    device_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(left_panel), device_list_box);

    /* Initial loading sync from local SQLite database file */
    reload_left_panel_devices();

    GtkWidget *btn_all_new = gtk_button_new_with_label("➕ Add New Gadget");
    gtk_button_set_has_frame(GTK_BUTTON(btn_all_new), FALSE);
    GtkWidget *lbl_new_child = gtk_button_get_child(GTK_BUTTON(btn_all_new));
    if (GTK_IS_LABEL(lbl_new_child)) gtk_label_set_xalign(GTK_LABEL(lbl_new_child), 0.0);
    gtk_widget_set_margin_start(btn_all_new, 10);
    gtk_widget_set_margin_top(btn_all_new, 15);
    gtk_box_append(GTK_BOX(left_panel), btn_all_new);

    /* Clean structural window panel layout separator */
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(main_box), separator);

    /* --- Right Panel Layout (Workspace area) --- */
    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(right_panel, TRUE);
    gtk_box_append(GTK_BOX(main_box), right_panel);

    GtkWidget *lbl_main = gtk_label_new("Please connect a mobile device or select an existing one from the sidebar list.");
    gtk_box_append(GTK_BOX(right_panel), lbl_main);

    /* Setup 1Hz continuous automated hardware probing tracking timer */
    g_timeout_add(1000, on_usb_tick, NULL);
    gtk_window_present(GTK_WINDOW(window));
}
