#ifndef UI_DASHBOARD_H
#define UI_DASHBOARD_H

#include <gtk/gtk.h>
#include "device_info.h"

/* Shared references to active dashboard widgets across modules */
extern GtkWidget *lbl_model, *lbl_os, *lbl_battery, *lbl_serial, *btn_serial_toggle;
extern GtkWidget *lbl_storage_text, *progress_storage, *grid_data, *box_storage;
extern GtkWidget *lbl_cat_system, *lbl_cat_apps, *lbl_cat_media;
extern char raw_serial_backup[64];
extern bool is_serial_masked;

/* Interface routines */
GtkWidget *create_device_dashboard(void);
void update_device_dashboard(const DeviceInfo *info);
void refresh_serial_display(void);

#endif /* UI_DASHBOARD_H */
