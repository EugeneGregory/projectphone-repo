#ifndef MEDIA_TYPES_H
#define MEDIA_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_FOLDER_NAME 64
#define MAX_VIRTUAL_FOLDERS 16

typedef struct {
    char name[MAX_FOLDER_NAME];
    char sample_path[256]; /* New slot to store the real physical hardware path */
    size_t photos_count;
    uint64_t photos_size;       
    size_t videos_count;
    uint64_t videos_size;       
    size_t total_count;
    uint64_t total_size;
} VirtualFolder;

typedef struct {
    size_t total_photos;
    uint64_t total_photos_size;
    size_t total_videos;
    uint64_t total_videos_size;
    
    VirtualFolder folders[MAX_VIRTUAL_FOLDERS];
    size_t folders_count;
    
    bool is_scanned;            
} MediaRegistry;

#endif // MEDIA_TYPES_H
