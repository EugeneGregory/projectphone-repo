#ifndef UI_MEDIA_TABS_H
#define UI_MEDIA_TABS_H

#include <gtk/gtk.h>
#include "media_types.h"

/*
 * Rebuilds the Photos & Videos tab inside GtkNotebook.
 * Replaces the old static placeholder with the new layout.
 */
GtkWidget *ui_media_tabs_create(MediaRegistry *registry);

/*
 * Dynamically updates the media statistics header and triggers the folder grid redraw.
 */
void ui_media_tabs_update(GtkWidget *tab_container, MediaRegistry *registry, const char *mount_path);

#endif // UI_MEDIA_TABS_H
