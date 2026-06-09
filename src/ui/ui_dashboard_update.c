#include "ui_dashboard.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void refresh_serial_display(void) {
    if (!lbl_serial || strlen(raw_serial_backup) == 0) return;
    if (is_serial_masked) {
        size_t len = strlen(raw_serial_backup);
        char masked[192] = {0};
        if (len > 60) len = 60;
        for (size_t i = 0; i < len; i++) strcpy(&masked[i*3], "•");
        gtk_label_set_text(GTK_LABEL(lbl_serial), masked);
        gtk_button_set_label(GTK_BUTTON(btn_serial_toggle), "👁️ Show");
    } else {
        gtk_label_set_text(GTK_LABEL(lbl_serial), raw_serial_backup);
        gtk_button_set_label(GTK_BUTTON(btn_serial_toggle), "🔒 Hide");
    }
}

void update_device_dashboard(const DeviceInfo *info) {
    if (!lbl_model) return;
    if (!info) {
        gtk_label_set_text(GTK_LABEL(lbl_model), "Please connect a mobile device or select an existing one from the sidebar list.");
        gtk_widget_set_visible(grid_data, FALSE);
        gtk_widget_set_visible(box_storage, FALSE);
        memset(raw_serial_backup, 0, sizeof(raw_serial_backup));
        return;
    }

    gtk_label_set_text(GTK_LABEL(lbl_model), info->model_name);
    gtk_widget_set_visible(grid_data, TRUE);
    gtk_widget_set_visible(box_storage, TRUE);

    char os_buf[64], sys_buf[128], apps_buf[128], med_buf[128], st_buf[128];
    snprintf(os_buf, sizeof(os_buf), "%s (%s)", info->os_version, info->is_ios ? "iOS" : "Android");
    gtk_label_set_text(GTK_LABEL(lbl_os), os_buf);
    gtk_label_set_text(GTK_LABEL(lbl_battery), info->battery_level);

    strncpy(raw_serial_backup, info->serial_number, sizeof(raw_serial_backup) - 1);
    refresh_serial_display();

    double total = atof(info->storage_total);
    double free_sp = atof(info->storage_free);
    if (total > 0.0) {
        double used = total - free_sp;
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_storage), used / total);
        snprintf(st_buf, sizeof(st_buf), "Used: %.1f GB / Total: %.1f GB (Free: %.1f GB)", used, total, free_sp);
        gtk_label_set_text(GTK_LABEL(lbl_storage_text), st_buf);

        snprintf(sys_buf, sizeof(sys_buf), "  └── ⚙️ System Memory: %s", strlen(info->storage_system) ? info->storage_system : "0.0 GB");
        snprintf(apps_buf, sizeof(apps_buf), "  └── 📦 Applications Size: %s", strlen(info->storage_apps) ? info->storage_apps : "0.0 GB");
        snprintf(med_buf, sizeof(med_buf), "  └── 🖼️ Media and Photos: %s", strlen(info->storage_media) ? info->storage_media : "0.0 GB");
        gtk_label_set_text(GTK_LABEL(lbl_cat_system), sys_buf);
        gtk_label_set_text(GTK_LABEL(lbl_cat_apps), apps_buf);
        gtk_label_set_text(GTK_LABEL(lbl_cat_media), med_buf);
    } else {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_storage), 0.0);
        gtk_label_set_text(GTK_LABEL(lbl_storage_text), "Storage metrics unavailable.");
    }
}
