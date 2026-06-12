#include "media_scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

static void string_to_lower(const char *src, char *dst, size_t max_len) {
    size_t i = 0;
    while (src[i] && i < max_len - 1) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

static bool is_photo_extension(const char *ext) {
    return (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0 || 
            strcmp(ext, "png") == 0 || strcmp(ext, "heic") == 0 || 
            strcmp(ext, "heif") == 0 || strcmp(ext, "aae") == 0);
}

static bool is_video_extension(const char *ext) {
    return (strcmp(ext, "mov") == 0 || strcmp(ext, "mp4") == 0 || 
            strcmp(ext, "m4v") == 0 || strcmp(ext, "3gp") == 0 || 
            strcmp(ext, "mkv") == 0);
}

static void detect_folder_name(const char *path, char *folder_name) {
    char lower_path[1024];
    string_to_lower(path, lower_path, sizeof(lower_path));

    if (strstr(lower_path, "telegram")) {
        strncpy(folder_name, "Telegram", MAX_FOLDER_NAME - 1);
    } else if (strstr(lower_path, "whatsapp")) {
        strncpy(folder_name, "WhatsApp", MAX_FOLDER_NAME - 1);
    } else if (strstr(lower_path, "viber")) {
        strncpy(folder_name, "Viber", MAX_FOLDER_NAME - 1);
    } else if (strstr(lower_path, "dcim")) {
        const char *apple_dir = strstr(lower_path, "dcim/");
        if (apple_dir && strlen(apple_dir) > 5) {
            char raw_name[64] = {0};
            strncpy(raw_name, apple_dir + 5, 60);
            char *slash = strchr(raw_name, '/');
            if (slash) *slash = '\0';
            
            /* Keep human label uppercase for clean rendering consistency */
            for(int i=0; raw_name[i]; i++) raw_name[i] = (char)toupper((unsigned char)raw_name[i]);
            snprintf(folder_name, MAX_FOLDER_NAME, "Camera (%s)", raw_name);
        } else {
            strncpy(folder_name, "Camera", MAX_FOLDER_NAME - 1);
        }
    } else {
        char raw_group[64] = {0};
        const char *root_dir = strstr(lower_path, "mnt_ios/");
        if (root_dir && strlen(root_dir) > 8) {
            strncpy(raw_group, root_dir + 8, 60);
            char *slash = strchr(raw_group, '/');
            if (slash) *slash = '\0';
            snprintf(folder_name, MAX_FOLDER_NAME, "Sys: %s", raw_group);
        } else {
            strncpy(folder_name, "Other Media", MAX_FOLDER_NAME - 1);
        }
    }
    folder_name[MAX_FOLDER_NAME - 1] = '\0';
}

static void registry_add_file(MediaRegistry *registry, const char *path, uint64_t size, bool is_photo) {
    char folder_name[MAX_FOLDER_NAME];
    char sample_path[1024] = {0};
    
    /* Crucial Fix: Extract exact casing preserved target tokens straight from device path */
    const char *sub = strstr(path, "mnt_ios/");
    if (sub) {
        strncpy(sample_path, sub + 8, sizeof(sample_path) - 1);
    } else {
        strncpy(sample_path, "root", sizeof(sample_path) - 1);
    }

    detect_folder_name(path, folder_name);

    if (is_photo) {
        registry->total_photos++;
        registry->total_photos_size += size;
    } else {
        registry->total_videos++;
        registry->total_videos_size += size;
    }

    for (size_t i = 0; i < registry->folders_count; i++) {
        if (strcmp(registry->folders[i].name, folder_name) == 0) {
            if (is_photo) {
                registry->folders[i].photos_count++;
                registry->folders[i].photos_size += size;
            } else {
                registry->folders[i].videos_count++;
                registry->folders[i].videos_size += size;
            }
            registry->folders[i].total_count++;
            registry->folders[i].total_size += size;
            return;
        }
    }

    if (registry->folders_count < MAX_VIRTUAL_FOLDERS) {
        size_t idx = registry->folders_count;
        memset(&registry->folders[idx], 0, sizeof(VirtualFolder));
        strncpy(registry->folders[idx].name, folder_name, MAX_FOLDER_NAME - 1);
        strncpy(registry->folders[idx].sample_path, sample_path, sizeof(registry[idx].folders[idx].sample_path) - 1);
        
        if (is_photo) {
            registry->folders[idx].photos_count = 1;
            registry->folders[idx].photos_size = size;
        } else {
            registry->folders[idx].videos_count = 1;
            registry->folders[idx].videos_size = size;
        }
        registry->folders[idx].total_count = 1;
        registry->folders[idx].total_size = size;
        
        registry->folders_count++;
    }
}

static void scan_directory_recursive(const char *dir_path, MediaRegistry *registry) {
    DIR *dir = opendir(dir_path);
    if (!dir) return;

    struct dirent *entry;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        DIR *sub_dir = opendir(path);
        if (sub_dir) {
            closedir(sub_dir);
            scan_directory_recursive(path, registry);
        } else {
            const char *dot = strrchr(entry->d_name, '.');
            if (dot) {
                char ext[16];
                string_to_lower(dot + 1, ext, sizeof(ext));

                struct stat st;
                uint64_t file_size = 0;
                if (stat(path, &st) == 0) {
                    file_size = (uint64_t)st.st_size;
                }

                if (is_photo_extension(ext)) {
                    registry_add_file(registry, path, file_size, true);
                } else if (is_video_extension(ext)) {
                    registry_add_file(registry, path, file_size, false);
                }
            }
        }
    }
    closedir(dir);
}

void media_scanner_run(const char *mount_path, MediaRegistry *registry) {
    if (!mount_path || !registry) return;
    media_scanner_reset(registry);
    scan_directory_recursive(mount_path, registry);
    registry->is_scanned = true;
}

void media_scanner_reset(MediaRegistry *registry) {
    if (!registry) return;
    memset(registry, 0, sizeof(MediaRegistry));
}
