#include "ui.h"
#include "dependencies.h"

/* Universal row builder rendering label, button control, and status indicator icon */
void add_dependency_row(GtkWidget *container, const SystemDependency *dep) {
    GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_bottom(row_box, 5);
    
    GtkWidget *lbl_name = gtk_label_new(dep->visible_name);
    gtk_widget_set_halign(lbl_name, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl_name, TRUE);
    gtk_box_append(GTK_BOX(row_box), lbl_name);
    
    int installed = is_dependency_installed(dep);
    GtkWidget *action_btn;
    GtkWidget *lbl_status;
    
    if (installed) {
        action_btn = gtk_button_new_with_label("🗑️");
        g_signal_connect(action_btn, "clicked", G_CALLBACK(on_dependency_remove_clicked), (gpointer)dep);
        lbl_status = gtk_label_new("✅");
    } else {
        action_btn = gtk_button_new_with_label("📥 Download");
        g_signal_connect(action_btn, "clicked", G_CALLBACK(on_dependency_download_clicked), (gpointer)dep);
        lbl_status = gtk_label_new("❌");
    }
    
    gtk_box_append(GTK_BOX(row_box), action_btn);
    gtk_box_append(GTK_BOX(row_box), lbl_status);
    gtk_box_append(GTK_BOX(container), row_box);
}
