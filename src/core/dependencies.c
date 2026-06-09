#include "dependencies.h"
#include <stdio.h>
#include <stdlib.h>

/* 
 * Checks if a specific system dependency is available in the OS.
 * Spawns a shell command using 'which' to locate the binary.
 * Returns 1 if installed, 0 if missing.
 */
int is_dependency_installed(const SystemDependency *dep) {
    char check_cmd[256];
    
    /* Format the shell check string safely redirection output to /dev/null */
    snprintf(check_cmd, sizeof(check_cmd), "which %s > /dev/null 2>&1", dep->check_command);
    
    if (system(check_cmd) == 0) {
        return 1; /* Component found */
    }
    
    return 0; /* Component missing */
}
