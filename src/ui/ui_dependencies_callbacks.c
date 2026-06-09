#include "ui.h"
#include "dependencies.h"
#include <stdlib.h>
#include <string.h>

/* Callback triggered when an individual download button is pressed */
void on_dependency_download_clicked(GtkButton *btn, gpointer user_data) {
    const SystemDependency *dep = (const SystemDependency *)user_data;
    char terminal_cmd[4096];
    
    g_print("[Click]: Installing missing system dependency: %s\n", dep->package_name);
    
    snprintf(terminal_cmd, sizeof(terminal_cmd),
             "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Installing system package %s...\"; "
             "sudo apt update && sudo apt install -y %s; read'", dep->package_name, dep->package_name);
             
    system(terminal_cmd);
    refresh_dependencies_window();
}

/* Callback triggered when an individual trash remove button is pressed */
void on_dependency_remove_clicked(GtkButton *btn, gpointer user_data) {
    const SystemDependency *dep = (const SystemDependency *)user_data;
    char terminal_cmd[4096];
    
    g_print("[Click]: Purging system dependency: %s\n", dep->package_name);
    
    snprintf(terminal_cmd, sizeof(terminal_cmd),
             "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Removing system package %s...\"; "
             "sudo apt purge -y %s; read'", dep->package_name, dep->package_name);
             
    system(terminal_cmd);
    refresh_dependencies_window();
}

/* Unified callback handling bulk setup actions (Install All / Remove All) */
void on_bulk_action(GtkButton *btn, gpointer user_data) {
    /* Extract data: category type from high 16 bits, action type from low 16 bits */
    int packed_data = GPOINTER_TO_INT(user_data);
    DependencyType target_type = (packed_data >> 16) & 0xFFFF;
    int action_type = packed_data & 0xFFFF; /* 1 = Install, 2 = Purge */
    
    char pkg_list[2048] = {0};
    int target_count = 0;
    
    for (int i = 0; i < DEPS_COUNT; i++) {
        if (SYSTEM_DEPS[i].type == target_type) {
            int installed = is_dependency_installed(&SYSTEM_DEPS[i]);
            
            if (action_type == 1 && !installed) {
                strcat(pkg_list, SYSTEM_DEPS[i].package_name);
                strcat(pkg_list, " ");
                target_count++;
            } else if (action_type == 2 && installed) {
                strcat(pkg_list, SYSTEM_DEPS[i].package_name);
                strcat(pkg_list, " ");
                target_count++;
            }
        }
    }
    
    if (target_count == 0) {
        g_print("[System]: No matching target components found for this bulk operational request.\n");
        return;
    }
    
    /* Expanded buffer size blocks compile-time truncation warning loops */
    char terminal_cmd[8192];
    if (action_type == 1) {
        snprintf(terminal_cmd, sizeof(terminal_cmd),
                 "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Executing bulk environment package installation: %s...\"; "
                 "sudo apt update && sudo apt install -y %s; read'", pkg_list, pkg_list);
    } else {
        snprintf(terminal_cmd, sizeof(terminal_cmd),
                 "gnome-terminal --wait -- bash -c 'echo \"[BlueCat Studio]: Executing safe bulk package purge routine: %s...\"; "
                 "sudo apt purge -y %s; read'", pkg_list, pkg_list);
    }
    
    system(terminal_cmd);
    refresh_dependencies_window();
}
