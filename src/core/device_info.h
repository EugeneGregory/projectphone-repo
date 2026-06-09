#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <stdbool.h>

/* Structure holding extracted live smartphone telemetry */
typedef struct {
    char model_name[64];
    char os_version[32];
    char serial_number[64];
    char battery_level[16];
    char storage_total[32];
    char storage_free[32];
    char storage_system[32]; /* System files size */
    char storage_apps[32];   /* User applications size */
    char storage_media[32];  /* Photos and media files size */
    bool is_ios;
} DeviceInfo;

void fetch_ios_device_info(DeviceInfo *info);
void fetch_android_device_info(DeviceInfo *info);

#endif /* DEVICE_INFO_H */
