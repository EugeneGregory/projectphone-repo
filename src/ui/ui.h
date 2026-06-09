#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include "dependencies.h"

/* Core application visual window components builders */
void build_ui(GtkApplication *app);
void reload_left_panel_devices(void);
gboolean on_usb_tick(gpointer user_data);

/* Environment packages management routines layout triggers */
void show_dependencies_manager_window(GtkWidget *parent_window);
void on_dependency_download_clicked(GtkButton *btn, gpointer user_data);
void on_dependency_remove_clicked(GtkButton *btn, gpointer user_data);
void on_bulk_action(GtkButton *btn, gpointer user_data);

/* Categorized listings construction mapping nodes */
GtkWidget *build_categorized_dependencies_list(void);
void refresh_dependencies_window(void);
GtkWidget *create_dependency_row_widget(const SystemDependency *dep);

#endif /* UI_H */
