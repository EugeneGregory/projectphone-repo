#include <gtk/gtk.h>
#include "ui.h"
#include "dependencies.h"

GtkWidget *create_dependency_row_widget(const SystemDependency *dep) {
    GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_widget_set_margin_start(row_box, 10);
    gtk_widget_set_margin_end(row_box, 10);
    gtk_widget_set_margin_top(row_box, 4);
    gtk_widget_set_margin_bottom(row_box, 4);

    GtkWidget *lbl_name = gtk_label_new(dep->visible_name);
    gtk_widget_set_halign(lbl_name, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl_name, TRUE);
    gtk_box_append(GTK_BOX(row_box), lbl_name);

    int installed = is_dependency_installed(dep);

    if (!installed) {
        /* "📥 Download" button alignment */
        GtkWidget *btn_download = gtk_button_new_with_label("📥 Download");
        g_signal_connect(btn_download, "clicked", G_CALLBACK(on_dependency_download_clicked), (gpointer)dep);
        gtk_box_append(GTK_BOX(row_box), btn_download);

        GtkWidget *lbl_status = gtk_label_new("❌");
        gtk_box_append(GTK_BOX(row_box), lbl_status);
    } else {
        /* "🗑️" Purge icon button alignment */
        GtkWidget *btn_purge = gtk_button_new_with_label("🗑️");
        g_signal_connect(btn_purge, "clicked", G_CALLBACK(on_dependency_remove_clicked), (gpointer)dep);
        gtk_box_append(GTK_BOX(row_box), btn_purge);

        GtkWidget *lbl_status = gtk_label_new("✅");
        gtk_box_append(GTK_BOX(row_box), lbl_status);
    }

    return row_box;
}
