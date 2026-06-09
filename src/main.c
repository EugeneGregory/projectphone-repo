#include <gtk/gtk.h>
#include <stdio.h>
#include "ui.h"
#include "database.h"

/* Triggered when the GtkApplication signals an active launch runtime environment status */
static void activate(GtkApplication *app, gpointer user_data) {
    build_ui(app);
}

int main(int argc, char **argv) {
    /* Initialize local SQLite3 database file storage tables */
    if (!init_database()) {
        fprintf(stderr, "[Critical Error]: Database initialization failure.\n");
        return 1;
    }

    GtkApplication *app = gtk_application_new("com.bluecat.projectphone", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
