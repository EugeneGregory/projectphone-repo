#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define USB_DIR "/sys/bus/usb/devices"

static int check_device_vendor(const char *dev_path) {
    char vendor_file[512];
    char product_file[512];
    char vendor_id[16] = {0};
    char product_id[16] = {0};
    
    snprintf(vendor_file, sizeof(vendor_file), "%s/idVendor", dev_path);
    snprintf(product_file, sizeof(product_file), "%s/idProduct", dev_path);
    
    FILE *f_vendor = fopen(vendor_file, "r");
    if (!f_vendor) return 0;

    if (fgets(vendor_id, sizeof(vendor_id), f_vendor) != NULL) {
        vendor_id[strcspn(vendor_id, "\n")] = 0;
        
        if (strcmp(vendor_id, "05ac") == 0) {
            FILE *f_product = fopen(product_file, "r");
            if (f_product) {
                if (fgets(product_id, sizeof(product_id), f_product) != NULL) {
                    product_id[strcspn(product_id, "\n")] = 0;
                    long pid = strtol(product_id, NULL, 16);
                    
                    if (pid >= 0x12a0 && pid <= 0x12af) {
                        fclose(f_product);
                        fclose(f_vendor);
                        return 1; // iPhone
                    }
                }
                fclose(f_product);
            }
        }
        
        if (strcmp(vendor_id, "2717") == 0 || strcmp(vendor_id, "18d1") == 0 || 
            strcmp(vendor_id, "04e8") == 0 || strcmp(vendor_id, "22b8") == 0) {
            fclose(f_vendor);
            return 2; // Android
        }
    }
    
    fclose(f_vendor);
    return 0;
}

int scan_usb_ports() {
    DIR *dir = opendir(USB_DIR);
    struct dirent *entry;
    int status = 0;

    if (!dir) return 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", USB_DIR, entry->d_name);
            int res = check_device_vendor(full_path);
            if (res > 0) {
                status = res;
                break; 
            }
        }
    }
    closedir(dir);
    return status;
}

// Отдельные функции для ОДНОКРАТНОГО извлечения данных, когда это нужно интерфейсу
void get_iphone_real_data(char *serial, char *model, char *custom, size_t max_len) {
    FILE *pipe;
    char prod_type[64] = {0};

    pipe = popen("ideviceinfo -k UniqueDeviceID 2>/dev/null", "r");
    if (pipe) {
        if (fgets(serial, max_len, pipe)) serial[strcspn(serial, "\r\n")] = 0;
        pclose(pipe);
    }
    pipe = popen("ideviceinfo -k ProductType 2>/dev/null", "r");
    if (pipe) {
        if (fgets(prod_type, sizeof(prod_type), pipe)) prod_type[strcspn(prod_type, "\r\n")] = 0;
        pclose(pipe);
    }
    pipe = popen("ideviceinfo -k DeviceName 2>/dev/null", "r");
    if (pipe) {
        if (fgets(custom, max_len, pipe)) custom[strcspn(custom, "\r\n")] = 0;
        pclose(pipe);
    }

    if (strcmp(prod_type, "iPhone12,1") == 0) strncpy(model, "iPhone 11", max_len);
    else if (strcmp(prod_type, "iPhone14,5") == 0) strncpy(model, "iPhone 13", max_len);
    else if (strcmp(prod_type, "iPhone5,1") == 0 || strcmp(prod_type, "iPhone5,2") == 0) strncpy(model, "iPhone 5", max_len);
    else strncpy(model, "iPhone", max_len);

    if (strlen(serial) == 0) strncpy(serial, "UNKNOWN_IOS_SERIAL", max_len);
    if (strlen(custom) == 0) strncpy(custom, "iPhone", max_len);
}

void get_android_real_data(char *serial, char *model, char *custom, size_t max_len) {
    FILE *pipe;
    pipe = popen("adb get-serialno 2>/dev/null", "r");
    if (pipe) {
        if (fgets(serial, max_len, pipe)) serial[strcspn(serial, "\r\n")] = 0;
        pclose(pipe);
    }
    pipe = popen("adb shell getprop ro.product.marketname 2>/dev/null", "r");
    if (pipe) {
        if (fgets(model, max_len, pipe)) model[strcspn(model, "\r\n")] = 0;
        pclose(pipe);
    }
    pipe = popen("adb shell getprop net.hostname 2>/dev/null", "r");
    if (pipe) {
        if (fgets(custom, max_len, pipe)) custom[strcspn(custom, "\r\n")] = 0;
        pclose(pipe);
    }

    if (strlen(serial) == 0) strncpy(serial, "UNKNOWN_AND_SERIAL", max_len);
    if (strlen(model) == 0) strncpy(model, "Android Device", max_len);
    if (strlen(custom) == 0) strncpy(custom, "MyAndroid", max_len);
}
