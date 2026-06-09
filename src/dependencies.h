#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <gtk/gtk.h>

/* Dependency categories for UI filtering */
typedef enum {
    DEP_TYPE_IOS,
    DEP_TYPE_ANDROID,
    DEP_TYPE_COMMON
} DependencyType;

/* Unified structure describing a system dependency */
typedef struct {
    const char *visible_name;   /* Clean name for UI display */
    const char *package_name;   /* Package name for apt install / purge */
    const char *check_command;  /* Executable checked via 'which' */
    DependencyType type;        /* Category group */
} SystemDependency;

/* Unified registry of all external system tools */
static const SystemDependency SYSTEM_DEPS[] = {
    /* iOS Components (Apple) */
    {" • libimobiledevice (Основная связь)", "libimobiledevice-utils", "ideviceinfo", DEP_TYPE_IOS},
    {" • ifuse (Доступ к файловой системе)", "ifuse", "ifuse", DEP_TYPE_IOS},
    {" • idevicebackup2 (Резервное копирование)", "idevicebackup2", "idevicebackup2", DEP_TYPE_IOS},
    {" • UxPlay (Зеркалирование экрана AirPlay)", "uxplay", "uxplay", DEP_TYPE_IOS},

    /* Android Components */
    {" • adb (Драйвер отладки и связи)", "adb", "adb", DEP_TYPE_ANDROID},
    {" • jmtpfs (Доступ к файловой системе MTP)", "jmtpfs", "jmtpfs", DEP_TYPE_ANDROID},
    {" • go-mtpfs (Альтернативный быстрый доступ к файлам)", "go-mtpfs", "go-mtpfs", DEP_TYPE_ANDROID},
    {" • scrcpy (Зеркалирование экрана)", "scrcpy", "scrcpy", DEP_TYPE_ANDROID},

    /* Common System Components */
    {" • SQLite3 (Локальная база данных)", "sqlite3", "sqlite3", DEP_TYPE_COMMON},
    {" • LibCurl (Сетевой движок и облако)", "curl", "curl", DEP_TYPE_COMMON}
};

/* Macro to automatically calculate total dependencies count */
#define DEPS_COUNT (sizeof(SYSTEM_DEPS) / sizeof(SYSTEM_DEPS[0]))

/* Core verification logic prototypes */
int is_dependency_installed(const SystemDependency *dep);

#endif /* DEPENDENCIES_H */
