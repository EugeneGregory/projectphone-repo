#include "ui_media_grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

typedef struct {
    VirtualFolder *folder;
    GtkWidget *main_box;
    char target_dir_path[512];
} FolderClickContext;

static void string_to_lower_local(const char *src, char *dst, size_t max_len) {
    size_t i = 0;
    while (src[i] && i < max_len - 1) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

static bool is_valid_media_ext(const char *ext) {
    return (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0 || 
            strcmp(ext, "png") == 0 || strcmp(ext, "heic") == 0 || 
            strcmp(ext, "heif") == 0 || strcmp(ext, "aae") == 0 ||
            strcmp(ext, "mov") == 0 || strcmp(ext, "mp4") == 0 || 
            strcmp(ext, "m4v") == 0 || strcmp(ext, "3gp") == 0 || 
            strcmp(ext, "gif") == 0 || strcmp(ext, "mkv") == 0);
}

static void on_back_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *main_box = GTK_WIDGET(user_data);
    GtkWidget *folder_container = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "folder_container"));
    GtkWidget *file_list_view = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "file_list_view"));
    GtkWidget *stats_grid = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "stats_grid"));
    GtkWidget *sep = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "main_separator"));
    (void)button;

    if (folder_container && file_list_view && stats_grid && sep) {
        gtk_widget_set_visible(file_list_view, FALSE);
        gtk_widget_set_visible(stats_grid, TRUE);
        gtk_widget_set_visible(sep, TRUE);
        gtk_widget_set_visible(folder_container, TRUE);
    }
}

static void on_download_all_clicked(GtkButton *button, gpointer user_data) {
    FolderClickContext *ctx = (FolderClickContext *)user_data;
    if (!ctx) return;

    char sys_cmd[1024] = {0};
    system("mkdir -p /home/eugene254/Downloads/ProjectPhone_Backup 2>/dev/null");

    snprintf(sys_cmd, sizeof(sys_cmd), "cp -r %s /home/eugene254/Downloads/ProjectPhone_Backup/ 2>/dev/null &", ctx->target_dir_path);
    system(sys_cmd);

    gtk_button_set_label(button, "✅ Copying in Background...");
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
}

static void on_folder_clicked(GtkButton *button, gpointer user_data) {
    FolderClickContext *ctx = (FolderClickContext *)user_data;
    if (!ctx || !ctx->main_box || !ctx->folder) return;

    GtkWidget *main_box = ctx->main_box;
    VirtualFolder *folder = ctx->folder;
    (void)button;

    GtkWidget *folder_container = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "folder_container"));
    GtkWidget *file_list_view = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "file_list_view"));
    GtkWidget *stats_grid = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "stats_grid"));
    GtkWidget *sep = GTK_WIDGET(g_object_get_data(G_OBJECT(main_box), "main_separator"));

    if (!folder_container || !file_list_view || !stats_grid || !sep) return;

    GtkWidget *child = gtk_widget_get_first_child(file_list_view);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(file_list_view), child);
        child = next;
    }

    gtk_widget_set_visible(folder_container, FALSE);
    gtk_widget_set_visible(stats_grid, FALSE);
    gtk_widget_set_visible(sep, FALSE);
    gtk_widget_set_visible(file_list_view, TRUE);

    GtkWidget *header_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    
    GtkWidget *btn_back = gtk_button_new_with_label("🔙 Back to Folders");
    g_signal_connect(btn_back, "clicked", G_CALLBACK(on_back_clicked), main_box);
    gtk_box_append(GTK_BOX(header_hbox), btn_back);

    GtkWidget *btn_download = gtk_button_new_with_label("📥 Download All Files");
    g_signal_connect(btn_download, "clicked", G_CALLBACK(on_download_all_clicked), ctx);
    gtk_box_append(GTK_BOX(header_hbox), btn_download);

    char title_buf[256];
    /* Fix header label to reflect only matched files count */
    snprintf(title_buf, sizeof(title_buf), "📂 Browsing: %s", folder->name);
    GtkWidget *lbl_title = gtk_label_new(title_buf);
    gtk_box_append(GTK_BOX(header_hbox), lbl_title);
    gtk_box_append(GTK_BOX(file_list_view), header_hbox);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll, TRUE);

    GtkWidget *rows_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    
    char full_mount_path[512];
    snprintf(full_mount_path, sizeof(full_mount_path), "/home/eugene254/.projectphone/mnt_ios/%s", folder->sample_path);
    char *last_slash = strrchr(full_mount_path, '/');
    if (last_slash) *last_slash = '\0';

    strncpy(ctx->target_dir_path, full_mount_path, sizeof(ctx->target_dir_path) - 1);

    DIR *dir = opendir(full_mount_path);
    if (dir) {
        struct dirent *entry;
        int files_rendered = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            
            /* Verify extension before rendering row item */
            const char *dot = strrchr(entry->d_name, '.');
            if (dot) {
                char ext[16];
                string_to_lower_local(dot + 1, ext, sizeof(ext));
                if (!is_valid_media_ext(ext)) {
                    continue; /* Skip non-media chunks */
                }
            } else {
                continue; /* Skip system indices without extensions */
            }

            char row_buf[512];
            snprintf(row_buf, sizeof(row_buf), "  📄  %s", entry->d_name);
            GtkWidget *lbl_file = gtk_label_new(row_buf);
            gtk_widget_set_halign(lbl_file, GTK_ALIGN_START);
            gtk_widget_set_margin_start(lbl_file, 8);
            gtk_widget_set_margin_top(lbl_file, 2);
            gtk_widget_set_margin_bottom(lbl_file, 2);
            
            gtk_box_append(GTK_BOX(rows_vbox), lbl_file);
            files_rendered++;
        }
        closedir(dir);
        
        if (files_rendered == 0) {
            GtkWidget *lbl_more = gtk_label_new("  Empty directory shell inside target scope.");
            gtk_box_append(GTK_BOX(rows_vbox), lbl_more);
        }
    } else {
        GtkWidget *lbl_err = gtk_label_new("  ❌ Failed to open raw physical storage directory channel.");
        gtk_box_append(GTK_BOX(rows_vbox), lbl_err);
    }

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), rows_vbox);
    gtk_box_append(GTK_BOX(file_list_view), scroll);
}

static void format_grid_size(uint64_t bytes, char *out_str, size_t max_len) {
    double size = (double)bytes;
    if (bytes < 1024 * 1024) {
        snprintf(out_str, max_len, "%.1f KB", size / 1024.0);
    } else if (bytes < 1024 * 1024 * 1024) {
        snprintf(out_str, max_len, "%.1f MB", size / (1024.0 * 1024.0));
    } else {
        snprintf(out_str, max_len, "%.1f GB", size / (1024.0 * 1024.0 * 1024.0));
    }
}

void ui_media_grid_populate(GtkWidget *folder_container, MediaRegistry *registry) {
    if (!folder_container || !registry) return;

    GtkWidget *main_box = gtk_widget_get_parent(folder_container);

    GtkWidget *child = gtk_widget_get_first_child(folder_container);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(folder_container), child);
        child = next;
    }

    if (registry->folders_count == 0) {
        GtkWidget *lbl_no_media = gtk_label_new("No media directories discovered on this device.");
        gtk_widget_set_halign(lbl_no_media, GTK_ALIGN_CENTER);
        gtk_widget_set_margin_top(lbl_no_media, 40);
        gtk_box_append(GTK_BOX(folder_container), lbl_no_media);
        return;
    }

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll, TRUE);

    GtkWidget *flowbox = gtk_flow_box_new();
    gtk_widget_set_valign(flowbox, GTK_ALIGN_START);
    gtk_widget_set_halign(flowbox, GTK_ALIGN_START);
    
    gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(flowbox), 3);
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 3);
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);
    gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(flowbox), 16);
    gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(flowbox), 16);

    for (size_t i = 0; i < registry->folders_count; i++) {
        char title_buf[256];
        char size_buf[64];
        char tooltip_buf[512];
        
        format_grid_size(registry->folders[i].total_size, size_buf, sizeof(size_buf));
        
        snprintf(title_buf, sizeof(title_buf), "📁 %s\n%lu files (%s)", 
                 registry->folders[i].name, 
                 registry->folders[i].total_count, 
                 size_buf);

        GtkWidget *btn_folder = gtk_button_new_with_label(title_buf);
        gtk_widget_set_size_request(btn_folder, 180, 75);
        
        snprintf(tooltip_buf, sizeof(tooltip_buf), "📍 Real path: /%s", registry->folders[i].sample_path);
        gtk_widget_set_tooltip_text(btn_folder, tooltip_buf);
        
        FolderClickContext *ctx = g_new0(FolderClickContext, 1);
        ctx->folder = &registry->folders[i];
        ctx->main_box = main_box;
        
        g_object_set_data_full(G_OBJECT(btn_folder), "click_ctx", ctx, g_free);
        
        g_signal_connect(btn_folder, "clicked", G_CALLBACK(on_folder_clicked), ctx);

        gtk_flow_box_insert(GTK_FLOW_BOX(flowbox), btn_folder, -1);
    }

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), flowbox);
    gtk_box_append(GTK_BOX(folder_container), scroll);
}

