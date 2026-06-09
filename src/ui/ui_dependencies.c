#include "ui.h"
#include "dependencies.h"

/* Global container allocations linked into the refresh routine */
GtkWidget *global_ios_box = NULL;
GtkWidget *global_android_box = NULL;
GtkWidget *global_common_box = NULL;

/* Generates and maps the complete modal pop-up manager interface frame structure */
void show_dependencies_manager_window(GtkWidget *main_window) {
    GtkWidget *dependency_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dependency_window), "Dependencies Management");
    gtk_window_set_default_size(GTK_WINDOW(dependency_window), 650, 620);
    gtk_window_set_transient_for(GTK_WINDOW(dependency_window), GTK_WINDOW(main_window));
    gtk_window_set_modal(GTK_WINDOW(dependency_window), TRUE);
    
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(content_box, 15);
    gtk_widget_set_margin_end(content_box, 15);
    gtk_widget_set_margin_top(content_box, 15);
    gtk_widget_set_margin_bottom(content_box, 15);
    gtk_window_set_child(GTK_WINDOW(dependency_window), content_box);
    
    /* Allocating isolated vertical container boxes */
    global_ios_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    global_android_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    global_common_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    /* --- Category Section 1: iOS --- */
    GtkWidget *ios_header = create_category_header("🍏 iOS Components (Apple)", DEP_TYPE_IOS);
    gtk_box_append(GTK_BOX(content_box), ios_header);
    gtk_box_append(GTK_BOX(content_box), global_ios_box);
    
    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep1, 10);
    gtk_widget_set_margin_bottom(sep1, 10);
    gtk_box_append(GTK_BOX(content_box), sep1);
    
    /* --- Category Section 2: Android --- */
    GtkWidget *and_header = create_category_header("🤖 Android Components", DEP_TYPE_ANDROID);
    gtk_box_append(GTK_BOX(content_box), and_header);
    gtk_box_append(GTK_BOX(content_box), global_android_box);
    
    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep2, 10);
    gtk_widget_set_margin_bottom(sep2, 10);
    gtk_box_append(GTK_BOX(content_box), sep2);
    
    /* --- Category Section 3: Common System Components --- */
    GtkWidget *com_header = create_category_header("⚙️ Common System Components", DEP_TYPE_COMMON);
    gtk_box_append(GTK_BOX(content_box), com_header);
    gtk_box_append(GTK_BOX(content_box), global_common_box);
    
    /* Automated registry loop mapping rows to target section boxes */
    for (int i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == DEP_TYPE_IOS) {
            add_dependency_row(global_ios_box, &SYSTEM_DEPS[i]);
        } else if (SYSTEM_DEPS[i].type == DEP_TYPE_ANDROID) {
            add_dependency_row(global_android_box, &SYSTEM_DEPS[i]);
        } else if (SYSTEM_DEPS[i].type == DEP_TYPE_COMMON) {
            add_dependency_row(global_common_box, &SYSTEM_DEPS[i]);
        }
    }
    
    gtk_window_present(GTK_WINDOW(dependency_window));
}
