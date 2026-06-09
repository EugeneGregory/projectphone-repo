#include <gtk/gtk.h>
#include <string.h>
#include "ui.h"
#include "database.h"

/* Temporary USB hardware detector hooks from usb_detector.c */
int scan_usb_ports(void); 
void get_iphone_real_data(char *serial, char *model, char *custom, size_t max_len);
void get_android_real_data(char *serial, char *model, char *custom, size_t max_len);

extern GtkWidget *device_list_box;
static int last_usb_status = 0; 

typedef struct {
    char vendor_id[16];
    GtkWidget *button_widget;
} DeviceWidgetMap;

/* Array allocation managing up to 20 saved sidebar devices */
static DeviceWidgetMap saved_buttons[20]; 
static int saved_buttons_count = 0;

/* Populates a single registered device button element into the sidebar vertical container */
static void add_device_button_to_ui(const char *vid, const char *brand, const char *model, const char *custom_name) {
    if (saved_buttons_count >= 20) return;

    char full_label[128];
    snprintf(full_label, sizeof(full_label), "%s %s (%s)", brand, model, custom_name);

    GtkWidget *btn = gtk_button_new_with_label(full_label);
    gtk_button_set_has_frame(GTK_BUTTON(btn), FALSE);
    
    GtkWidget *child_label = gtk_button_get_child(GTK_BUTTON(btn));
    if (GTK_IS_LABEL(child_label)) {
        gtk_label_set_xalign(GTK_LABEL(child_label), 0.0);
    }
    gtk_widget_set_margin_start(btn, 10);
    gtk_widget_set_margin_end(btn, 10);
    gtk_widget_set_margin_top(btn, 5);

    gtk_widget_set_sensitive(btn, FALSE);
    gtk_box_append(GTK_BOX(device_list_box), btn);

    strncpy(saved_buttons[saved_buttons_count].vendor_id, vid, 16);
    saved_buttons[saved_buttons_count].button_widget = btn;
    saved_buttons_count++;
}

/* Wipes the graphical left list container and refills it smoothly from SQLite */
void reload_left_panel_devices(void) {
    GtkWidget *child = gtk_widget_get_first_child(device_list_box);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(device_list_box), child);
        child = next;
    }
    
    /* Counter reset prevents memory out-of-bounds duplication bugs */
    saved_buttons_count = 0;
    
    load_saved_devices(add_device_button_to_ui);
}

/* 1Hz system hardware checking loop querying active USB ports */
gboolean on_usb_tick(gpointer user_data) {
    int current_status = scan_usb_ports();

    if (current_status != 0 && last_usb_status == 0) {
        last_usb_status = current_status; 
        
        char s[64] = {0}, m[64] = {0}, c[64] = {0};
        if (current_status == 1) {
            get_iphone_real_data(s, m, c, 64);
            if (strcmp(s, "UNKNOWN_IOS_SERIAL") != 0 && strstr(m, "Unknown") == NULL && strstr(c, "ERROR") == NULL) {
                register_new_device("05ac", s, "Apple", m, c);
                reload_left_panel_devices();
            } else {
                last_usb_status = 0; 
            }
        } else if (current_status == 2) {
            get_android_real_data(s, m, c, 64);
            if (strcmp(s, "UNKNOWN_AND_SERIAL") != 0) {
                register_new_device("2717", s, "Xiaomi", m, c);
                reload_left_panel_devices();
            } else {
                last_usb_status = 0;
            }
        }
    } else if (current_status == 0 && last_usb_status != 0) {
        last_usb_status = 0;
    }

    const char *active_vid = "";
    if (current_status == 1) active_vid = "05ac";
    if (current_status == 2) active_vid = "2717";

    for (int i = 0; i < saved_buttons_count; i++) {
        if (strcmp(saved_buttons[i].vendor_id, active_vid) == 0 || 
            (current_status == 2 && strcmp(saved_buttons[i].vendor_id, "05ac") != 0)) { 
            gtk_widget_set_sensitive(saved_buttons[i].button_widget, TRUE);
        } else {
            gtk_widget_set_sensitive(saved_buttons[i].button_widget, FALSE);
        }
    }
    return TRUE; 
}
