#ifndef UI_MEDIA_GRID_H
#define UI_MEDIA_GRID_H

#include <gtk/gtk.h>
#include "media_types.h"

/*
 * Clears the folder container and populates it with square/rectangular folder buttons
 * based on the discovered folders inside the registry.
 */
void ui_media_grid_populate(GtkWidget *folder_container, MediaRegistry *registry);

#endif // UI_MEDIA_GRID_H
