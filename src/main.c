#include <gtk/gtk.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ui.h"

static void activate(GtkApplication *app, gpointer user_data) {
    /* Explicitly suppress the unused parameter warning for clean compilation */
    (void)user_data;
    build_ui(app);
}

int main(int argc, char **argv) {
    /* Auto-create hidden sandbox infrastructure inside home directory for secure mounting */
    mkdir("/home/eugene254/.projectphone", 0755);
    mkdir("/home/eugene254/.projectphone/mnt_ios", 0755);
    mkdir("/home/eugene254/.projectphone/mnt_android", 0755);

    GtkApplication *app = gtk_application_new("com.github.eugene254.projectphone", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
