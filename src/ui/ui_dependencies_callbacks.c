#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "dependencies.h"

void on_dependency_download_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    const SystemDependency *dep = (const SystemDependency *)user_data;
    char command[256];
    snprintf(command, sizeof(command), 
             "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Installing package: %s...\"; "
             "sudo apt update && sudo apt install -y %s; echo \"Done! Press Enter to close.\"; read'", 
             dep->package_name, dep->package_name);
    int ret = system(command);
    (void)ret;
}

void on_dependency_remove_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    const SystemDependency *dep = (const SystemDependency *)user_data;
    char command[256];
    snprintf(command, sizeof(command), 
             "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Removing package: %s...\"; "
             "sudo apt purge -y %s && sudo apt autoremove -y; echo \"Done! Press Enter to close.\"; read'", 
             dep->package_name, dep->package_name);
    int ret = system(command);
    (void)ret;
}

void on_bulk_action(GtkButton *btn, gpointer user_data) {
    (void)btn;
    int is_install = GPOINTER_TO_INT(user_data);
    char packages[512] = {0};

    for (size_t i = 0; i < DEPS_COUNT; i++) {
        strcat(packages, SYSTEM_DEPS[i].package_name);
        strcat(packages, " ");
    }

    char command[1024];
    if (is_install) {
        snprintf(command, sizeof(command),
                 "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Installing all dependencies...\"; "
                 "sudo apt update && sudo apt install -y %s; read'", packages);
    } else {
        snprintf(command, sizeof(command),
                 "gnome-terminal -- bash -c 'echo \"[ProjectPhone] Purging all dependencies...\"; "
                 "sudo apt purge -y %s && sudo apt autoremove -y; read'", packages);
    }
    int ret = system(command);
    (void)ret;
}
