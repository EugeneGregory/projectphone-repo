#include <gtk/gtk.h>
#include "ui.h"
#include "dependencies.h"

GtkWidget *create_dependency_row_widget(const SystemDependency *dep);

static void on_group_action_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    long combo = (long)user_data;
    int is_install = combo & 1;
    DependencyType target_type = (DependencyType)(combo >> 1);

    /* Allocate proper character arrays for command string accumulation */
    char packages[512] = {0};
    for (size_t i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == target_type) {
            strcat(packages, SYSTEM_DEPS[i].package_name);
            strcat(packages, " ");
        }
    }

    char command[1024] = {0};
    if (is_install) {
        snprintf(command, sizeof(command),
                 "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Installing selected ecosystem group...\"; "
                 "sudo apt update && sudo apt install -y %s; read'", packages);
    } else {
        snprintf(command, sizeof(command),
                 "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Purging selected ecosystem group...\"; "
                 "sudo apt purge -y %s && sudo apt autoremove -y; read'", packages);
    }
    int ret = system(command);
    (void)ret;
}

static void render_section_header_with_actions(GtkWidget *box, const char *title, int dep_type) {
    GtkWidget *header_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_top(header_hbox, 14);
    gtk_widget_set_margin_bottom(header_hbox, 6);
    gtk_box_append(GTK_BOX(box), header_hbox);

    GtkWidget *lbl = gtk_label_new(title);
    gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    gtk_widget_set_hexpand(lbl, TRUE);
    PangoAttrList *attrs = pango_attr_list_new();
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(GTK_LABEL(lbl), attrs);
    pango_attr_list_unref(attrs);
    gtk_box_append(GTK_BOX(header_hbox), lbl);

    GtkWidget *btn_ins = gtk_button_new_with_label("📥 Install All");
    long ins_data = (dep_type << 1) | 1;
    g_signal_connect(btn_ins, "clicked", G_CALLBACK(on_group_action_clicked), (gpointer)ins_data);
    gtk_box_append(GTK_BOX(header_hbox), btn_ins);

    GtkWidget *btn_del = gtk_button_new_with_label("🗑️ Remove All");
    long del_data = (dep_type << 1) | 0;
    g_signal_connect(btn_del, "clicked", G_CALLBACK(on_group_action_clicked), (gpointer)del_data);
    gtk_box_append(GTK_BOX(header_hbox), btn_del);
}

void refresh_dependencies_window(void) {}

GtkWidget *build_categorized_dependencies_list(void) {
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    /* --- SECTION 1: iOS Components --- */
    render_section_header_with_actions(main_vbox, "🍏 iOS Components (Apple)", DEP_TYPE_IOS);
    GtkWidget *list_ios = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_ios), GTK_SELECTION_NONE);
    gtk_box_append(GTK_BOX(main_vbox), list_ios);
    for (size_t i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == DEP_TYPE_IOS) {
            gtk_list_box_append(GTK_LIST_BOX(list_ios), create_dependency_row_widget(&SYSTEM_DEPS[i]));
        }
    }

    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep1, 10);
    gtk_box_append(GTK_BOX(main_vbox), sep1);

    /* --- SECTION 2: Android Components --- */
    render_section_header_with_actions(main_vbox, "🤖 Android Components", DEP_TYPE_ANDROID);
    GtkWidget *list_and = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_and), GTK_SELECTION_NONE);
    gtk_box_append(GTK_BOX(main_vbox), list_and);
    for (size_t i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == DEP_TYPE_ANDROID) {
            gtk_list_box_append(GTK_LIST_BOX(list_and), create_dependency_row_widget(&SYSTEM_DEPS[i]));
        }
    }

    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep2, 10);
    gtk_box_append(GTK_BOX(main_vbox), sep2);

    /* --- SECTION 3: Common Systems --- */
    render_section_header_with_actions(main_vbox, "⚙️ Common System Components", DEP_TYPE_COMMON);
    GtkWidget *list_com = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_com), GTK_SELECTION_NONE);
    gtk_box_append(GTK_BOX(main_vbox), list_com);
    for (size_t i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == DEP_TYPE_COMMON) {
            gtk_list_box_append(GTK_LIST_BOX(list_com), create_dependency_row_widget(&SYSTEM_DEPS[i]));
        }
    }

    return main_vbox;
}
