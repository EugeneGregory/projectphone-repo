#ifndef MEDIA_SCANNER_H
#define MEDIA_SCANNER_H

#include "media_types.h"

/*
 * Recursively scans the target mount path, identifies media files
 * by extension, detects their source directories, and updates the registry.
 */
void media_scanner_run(const char *mount_path, MediaRegistry *registry);

/*
 * Resets all counters and clears the folder array in the registry.
 */
void media_scanner_reset(MediaRegistry *registry);

#endif // MEDIA_SCANNER_H
