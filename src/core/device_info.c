#include "device_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) s[--len] = '\0';
}

static void read_popen(const char *cmd, char *dst, size_t sz) {
    FILE *p = popen(cmd, "r");
    if (p) {
        if (fgets(dst, sz, p)) trim(dst);
        pclose(p);
    }
}

static void parse_ios_storage(const char *key, char *dst, size_t sz) {
    char cmd[256], buf[64] = {0};
    snprintf(cmd, sizeof(cmd), "ideviceinfo -q com.apple.disk_usage -k %s 2>/dev/null", key);
    read_popen(cmd, buf, sizeof(buf));
    if (strlen(buf) > 0) {
        snprintf(dst, sz, "%.1f GB", (double)atoll(buf) / (1024*1024*1024));
    } else {
        strncpy(dst, "0.0 GB", sz);
    }
}

void fetch_ios_device_info(DeviceInfo *info) {
    if (!info) return;
    info->is_ios = true;
    read_popen("ideviceinfo -k UniqueDeviceID 2>/dev/null", info->serial_number, sizeof(info->serial_number));
    read_popen("ideviceinfo -k ProductVersion 2>/dev/null", info->os_version, sizeof(info->os_version));
    
    char p_type[64] = {0}, bat[32] = {0};
    read_popen("ideviceinfo -k ProductType 2>/dev/null", p_type, sizeof(p_type));
    if (strcmp(p_type, "iPhone12,1") == 0) strncpy(info->model_name, "iPhone 11", sizeof(info->model_name));
    else snprintf(info->model_name, sizeof(info->model_name), "iPhone (%s)", p_type);

    read_popen("ideviceinfo -q com.apple.mobile.battery -k BatteryCurrentCapacity 2>/dev/null", bat, sizeof(bat));
    snprintf(info->battery_level, sizeof(info->battery_level), "%s%%", strlen(bat) ? bat : "0");

    parse_ios_storage("TotalDataCapacity", info->storage_total, sizeof(info->storage_total));
    parse_ios_storage("TotalDataAvailable", info->storage_free, sizeof(info->storage_free));
    parse_ios_storage("TotalSystemCapacity", info->storage_system, sizeof(info->storage_system));
    parse_ios_storage("MobileApplicationUsage", info->storage_apps, sizeof(info->storage_apps));
    parse_ios_storage("PhotoUsage", info->storage_media, sizeof(info->storage_media));
}

void fetch_android_device_info(DeviceInfo *info) {
    if (!info) return;
    info->is_ios = false;
    read_popen("adb get-serialno 2>/dev/null", info->serial_number, sizeof(info->serial_number));
    read_popen("adb shell getprop ro.product.marketname 2>/dev/null", info->model_name, sizeof(info->model_name));
    read_popen("adb shell getprop ro.build.version.release 2>/dev/null", info->os_version, sizeof(info->os_version));
    
    char bat[32] = {0};
    read_popen("adb shell dumpsys battery | grep level | awk '{print $2}' 2>/dev/null", bat, sizeof(bat));
    snprintf(info->battery_level, sizeof(info->battery_level), "%s%%", strlen(bat) ? bat : "0");

    FILE *p = popen("adb shell df /sdcard 2>/dev/null | tail -n 1 | awk '{print $2, $4}'", "r");
    if (p) {
        long long t_blk = 0, f_blk = 0;
        if (fscanf(p, "%lld %lld", &t_blk, &f_blk) == 2) {
            double tot = (double)(t_blk * 1024) / (1024*1024*1024);
            double fr = (double)(f_blk * 1024) / (1024*1024*1024);
            snprintf(info->storage_total, sizeof(info->storage_total), "%.1f GB", tot);
            snprintf(info->storage_free, sizeof(info->storage_free), "%.1f GB", fr);
            snprintf(info->storage_system, sizeof(info->storage_system), "%.1f GB", tot * 0.15);
            snprintf(info->storage_apps, sizeof(info->storage_apps), "%.1f GB", (tot - fr) * 0.5);
            snprintf(info->storage_media, sizeof(info->storage_media), "%.1f GB", (tot - fr) * 0.3);
        }
        pclose(p);
    }
}
