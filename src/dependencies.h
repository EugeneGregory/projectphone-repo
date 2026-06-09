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
    const char *visible_name; /* Clean name for UI display */
    const char *package_name; /* Package name for apt install / purge */
    const char *check_command; /* Executable checked via 'which' */
    DependencyType type; /* Category group */
} SystemDependency;

/* Unified registry of all external system tools */
static const SystemDependency SYSTEM_DEPS[] = {
    /* iOS Components (Apple) */
    {" • libimobiledevice (Core Communication)", "libimobiledevice-utils", "ideviceinfo", DEP_TYPE_IOS},
    {" • ifuse (Filesystem Access)", "ifuse", "ifuse", DEP_TYPE_IOS},
    {" • idevicebackup2 (Backup Utilities)", "idevicebackup2", "idevicebackup2", DEP_TYPE_IOS},
    {" • UxPlay (AirPlay Screen Mirroring)", "uxplay", "uxplay", DEP_TYPE_IOS},

    /* Android Components */
    {" • adb (Bridge and Debugging Driver)", "adb", "adb", DEP_TYPE_ANDROID},
    {" • jmtpfs (MTP Filesystem Access)", "jmtpfs", "jmtpfs", DEP_TYPE_ANDROID},
    {" • go-mtpfs (Alternative Fast File Access)", "go-mtpfs", "go-mtpfs", DEP_TYPE_ANDROID},
    {" • scrcpy (Screen Mirroring Tools)", "scrcpy", "scrcpy", DEP_TYPE_ANDROID},

    /* Common System Components */
    {" • SQLite3 (Local Database Storage)", "sqlite3", "sqlite3", DEP_TYPE_COMMON},
    {" • LibCurl (Network Engine and Cloud)", "curl", "curl", DEP_TYPE_COMMON}
};

/* Macro to automatically calculate total dependencies count */
#define DEPS_COUNT (sizeof(SYSTEM_DEPS) / sizeof(SYSTEM_DEPS[0]))

/* Core verification logic prototypes */
int is_dependency_installed(const SystemDependency *dep);

#endif /* DEPENDENCIES_H */
