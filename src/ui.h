#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include "dependencies.h"

/* Main workspace layout builder */
void build_ui(GtkApplication *app);

/* Dependencies manager window controls */
void show_dependencies_manager_window(GtkWidget *main_window);
void refresh_dependencies_window(void);

/* Universal widget row renderer for system dependencies */
void add_dependency_row(GtkWidget *container, const SystemDependency *dep);

/* Section helper from ui_dependencies_sections.c */
GtkWidget *create_category_header(const char *title, DependencyType type);

/* System execution callbacks from ui_dependencies_callbacks.c */
void on_dependency_download_clicked(GtkButton *btn, gpointer user_data);
void on_dependency_remove_clicked(GtkButton *btn, gpointer user_data);
void on_bulk_action(GtkButton *btn, gpointer user_data);

/* USB Timer interface routines from ui_usb_timer.c */
void reload_left_panel_devices(void);
gboolean on_usb_tick(gpointer user_data);

#endif /* UI_H */
