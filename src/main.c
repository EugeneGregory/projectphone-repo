#include <gtk/gtk.h>
#include <stdio.h>

void build_ui(GtkApplication *app);
int init_database();

static void activate(GtkApplication *app, gpointer user_data) {
    build_ui(app);
}

int main(int argc, char **argv) {
    printf("=========================================\n");
    printf("        PROJECTPHONE v0.1 (Alpha)        \n");
    printf("=========================================\n");
    
    // Инициализируем SQLite3
    if (!init_database()) {
        fprintf(stderr, "[Критическая ошибка]: Программа остановлена. Ошибка БД.\n");
        return 1;
    }

    GtkApplication *app = gtk_application_new("com.bluecat.projectphone", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
