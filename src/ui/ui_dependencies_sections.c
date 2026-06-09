#include "ui.h"
#include "dependencies.h"

/* Static references needed for the automated refresh loop */
extern GtkWidget *global_ios_box;
extern GtkWidget *global_android_box;
extern GtkWidget *global_common_box;

/* 
 * Automatically triggers UI redraws for all three dependency container sections
 * after any package modification process terminates in the shell.
 */
void refresh_dependencies_window(void) {
    g_print("[System]: Triggering automated dependencies list redraw...\n");
    
    if (global_ios_box && global_android_box && global_common_box) {
        GtkWidget *child;

        /* Rebuild iOS section */
        child = gtk_widget_get_first_child(global_ios_box);
        while (child != NULL) {
            GtkWidget *next = gtk_widget_get_next_sibling(child);
            gtk_box_remove(GTK_BOX(global_ios_box), child);
            child = next;
        }
        for (int i = 0; i < DEPS_COUNT; i++) {
            if (SYSTEM_DEPS[i].type == DEP_TYPE_IOS) {
                add_dependency_row(global_ios_box, &SYSTEM_DEPS[i]);
            }
        }

        /* Rebuild Android section */
        child = gtk_widget_get_first_child(global_android_box);
        while (child != NULL) {
            GtkWidget *next = gtk_widget_get_next_sibling(child);
            gtk_box_remove(GTK_BOX(global_android_box), child);
            child = next;
        }
        for (int i = 0; i < DEPS_COUNT; i++) {
            if (SYSTEM_DEPS[i].type == DEP_TYPE_ANDROID) {
                add_dependency_row(global_android_box, &SYSTEM_DEPS[i]);
            }
        }

        /* Rebuild Common section */
        child = gtk_widget_get_first_child(global_common_box);
        while (child != NULL) {
            GtkWidget *next = gtk_widget_get_next_sibling(child);
            gtk_box_remove(GTK_BOX(global_common_box), child);
            child = next;
        }
        for (int i = 0; i < DEPS_COUNT; i++) {
            if (SYSTEM_DEPS[i].type == DEP_TYPE_COMMON) {
                add_dependency_row(global_common_box, &SYSTEM_DEPS[i]);
            }
        }
    }
}

/* Helper to build a clean category header with bulk action triggers */
GtkWidget *create_category_header(const char *title, DependencyType type) {
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_bottom(header_box, 10);
    
    GtkWidget *lbl_title = gtk_label_new(title);
    gtk_widget_set_halign(lbl_title, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl_title, TRUE);
    
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl_title), attrs);
    pango_attr_list_unref(attrs);
    gtk_box_append(GTK_BOX(header_box), lbl_title);
    
    GtkWidget *btn_in = gtk_button_new_with_label("📥 Install All");
    g_signal_connect(btn_in, "clicked", G_CALLBACK(on_bulk_action), GINT_TO_POINTER((type << 16) | 1));
    gtk_box_append(GTK_BOX(header_box), btn_in);
    
    GtkWidget *btn_out = gtk_button_new_with_label("🗑️ Remove All");
    g_signal_connect(btn_out, "clicked", G_CALLBACK(on_bulk_action), GINT_TO_POINTER((type << 16) | 2));
    gtk_box_append(GTK_BOX(header_box), btn_out);
    
    return header_box;
}
