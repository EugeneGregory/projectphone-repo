#include <gtk/gtk.h>
#include "ui.h"
#include "dependencies.h"

void show_dependencies_manager_window(GtkWidget *parent_window) {
    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Environment Dependencies Management Panel");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 650, 520);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent_window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(vbox, 15);
    gtk_widget_set_margin_end(vbox, 15);
    gtk_widget_set_margin_top(vbox, 5);
    gtk_widget_set_margin_bottom(vbox, 15);
    gtk_window_set_child(GTK_WINDOW(dialog), vbox);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append(GTK_BOX(vbox), scrolled);

    GtkWidget *categorized_list = build_categorized_dependencies_list();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), categorized_list);

    gtk_window_present(GTK_WINDOW(dialog));
}
