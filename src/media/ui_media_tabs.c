#include "ui_media_tabs.h"
#include "media_scanner.h"
#include "ui_media_grid.h"
#include <stdio.h>
#include <string.h>

static void format_size(uint64_t bytes, char *out_str, size_t max_len) {
    double size = (double)bytes;
    if (bytes < 1024 * 1024) {
        snprintf(out_str, max_len, "%.1f KB", size / 1024.0);
    } else if (bytes < 1024 * 1024 * 1024) {
        snprintf(out_str, max_len, "%.1f MB", size / (1024.0 * 1024.0));
    } else {
        snprintf(out_str, max_len, "%.1f GB", size / (1024.0 * 1024.0 * 1024.0));
    }
}

GtkWidget *ui_media_tabs_create(MediaRegistry *registry) {
    (void)registry;

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(main_box, 16);
    gtk_widget_set_margin_end(main_box, 16);
    gtk_widget_set_margin_top(main_box, 16);
    gtk_widget_set_margin_bottom(main_box, 16);

    GtkWidget *stats_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(stats_grid), 40);
    gtk_grid_set_row_spacing(GTK_GRID(stats_grid), 6);
    g_object_set_data(G_OBJECT(main_box), "stats_grid", stats_grid);

    GtkWidget *lbl_photos = gtk_label_new("📸 Photos: 0");
    GtkWidget *lbl_photos_sz = gtk_label_new("⚖️ Size: 0 KB");
    GtkWidget *lbl_videos = gtk_label_new("🎥 Videos: 0");
    GtkWidget *lbl_videos_sz = gtk_label_new("⚖️ Size: 0 KB");
    GtkWidget *lbl_total = gtk_label_new("📊 Total Media: 0 files | 0 KB");

    gtk_widget_set_halign(lbl_photos, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_photos_sz, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_videos, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_videos_sz, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_total, GTK_ALIGN_START);

    gtk_grid_attach(GTK_GRID(stats_grid), lbl_photos, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_photos_sz, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_videos, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_videos_sz, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_total, 0, 2, 2, 1);

    gtk_box_append(GTK_BOX(main_box), stats_grid);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    g_object_set_data(G_OBJECT(main_box), "main_separator", separator);
    gtk_box_append(GTK_BOX(main_box), separator);

    /* Container 1: Grid View for Folders */
    GtkWidget *folder_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    g_object_set_data(G_OBJECT(main_box), "folder_container", folder_container);
    gtk_box_append(GTK_BOX(main_box), folder_container);

    /* Container 2: File List View Container (Hidden by default) */
    GtkWidget *file_list_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_visible(file_list_view, FALSE);
    g_object_set_data(G_OBJECT(main_box), "file_list_view", file_list_view);
    gtk_box_append(GTK_BOX(main_box), file_list_view);

    GtkWidget *lbl_empty = gtk_label_new("Please connect a device to scan media...");
    gtk_widget_set_halign(lbl_empty, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(lbl_empty, 40);
    gtk_box_append(GTK_BOX(folder_container), lbl_empty);

    return main_box;
}

void ui_media_tabs_update(GtkWidget *tab_container, MediaRegistry *registry, const char *mount_path) {
    if (!tab_container || !registry) return;

    if (!registry->is_scanned && mount_path) {
        media_scanner_run(mount_path, registry);
    }

    GtkWidget *stats_grid = GTK_WIDGET(g_object_get_data(G_OBJECT(tab_container), "stats_grid"));
    GtkWidget *folder_container = GTK_WIDGET(g_object_get_data(G_OBJECT(tab_container), "folder_container"));

    if (!stats_grid || !folder_container) return;

    GtkWidget *child = gtk_widget_get_first_child(stats_grid);
    while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_grid_remove(GTK_GRID(stats_grid), child);
        child = next;
    }

    char p_size_str[32], v_size_str[32], t_size_str[32];
    format_size(registry->total_photos_size, p_size_str, sizeof(p_size_str));
    format_size(registry->total_videos_size, v_size_str, sizeof(v_size_str));
    format_size(registry->total_photos_size + registry->total_videos_size, t_size_str, sizeof(t_size_str));

    char buf[128];
    snprintf(buf, sizeof(buf), "📸 Photos: %lu", registry->total_photos);
    GtkWidget *lbl_photos = gtk_label_new(buf);
    
    snprintf(buf, sizeof(buf), "⚖️ Size: %s", p_size_str);
    GtkWidget *lbl_photos_sz = gtk_label_new(buf);

    snprintf(buf, sizeof(buf), "🎥 Videos: %lu", registry->total_videos);
    GtkWidget *lbl_videos = gtk_label_new(buf);

    snprintf(buf, sizeof(buf), "⚖️ Size: %s", v_size_str);
    GtkWidget *lbl_videos_sz = gtk_label_new(buf);

    snprintf(buf, sizeof(buf), "📊 Total Media: %lu files | %s", 
             registry->total_photos + registry->total_videos, t_size_str);
    GtkWidget *lbl_total = gtk_label_new(buf);

    gtk_widget_set_halign(lbl_photos, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_photos_sz, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_videos, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_videos_sz, GTK_ALIGN_START);
    gtk_widget_set_halign(lbl_total, GTK_ALIGN_START);

    gtk_grid_attach(GTK_GRID(stats_grid), lbl_photos, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_photos_sz, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_videos, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_videos_sz, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(stats_grid), lbl_total, 0, 2, 2, 1);

    ui_media_grid_populate(folder_container, registry);
}
