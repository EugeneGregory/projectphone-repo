#include <gtk/gtk.h>

void build_ui_ios(GtkWidget *container);
void build_ui_android(GtkWidget *container);
void build_ui_common(GtkWidget *container);

static GtkWidget *global_ios_box = NULL;
static GtkWidget *global_android_box = NULL;
static GtkWidget *global_common_box = NULL;

// Глобальная функция обновления, которую вызывают плагины после закрытия терминала
void refresh_plugins_window() {
    g_print("[Система]: Запущено автоматическое обновление списков плагинов...\n");
    if (global_ios_box && global_android_box && global_common_box) {
        build_ui_ios(global_ios_box);
        build_ui_android(global_android_box);
        build_ui_common(global_common_box);
    }
}

// Функция, которая строит само всплывающее модальное окно плагинов
void show_plugins_manager_window(GtkWidget *main_window) {
    GtkWidget *plugin_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(plugin_window), "Менеджер плагинов");
    gtk_window_set_default_size(GTK_WINDOW(plugin_window), 650, 620);
    
    gtk_window_set_transient_for(GTK_WINDOW(plugin_window), GTK_WINDOW(main_window));
    gtk_window_set_modal(GTK_WINDOW(plugin_window), TRUE);

    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(content_box, 15);
    gtk_widget_set_margin_end(content_box, 15);
    gtk_widget_set_margin_top(content_box, 15);
    gtk_widget_set_margin_bottom(content_box, 15);
    gtk_window_set_child(GTK_WINDOW(plugin_window), content_box);

    global_ios_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    global_android_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    global_common_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Блок 1: iOS
    gtk_box_append(GTK_BOX(content_box), global_ios_box);
    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep1, 10);
    gtk_widget_set_margin_bottom(sep1, 10);
    gtk_box_append(GTK_BOX(content_box), sep1);

    // Блок 2: Android
    gtk_box_append(GTK_BOX(content_box), global_android_box);
    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep2, 10);
    gtk_widget_set_margin_bottom(sep2, 10);
    gtk_box_append(GTK_BOX(content_box), sep2);

    // Блок 3: Общие утилиты
    gtk_box_append(GTK_BOX(content_box), global_common_box);

    // Отрисовка контента из трёх модулей
    build_ui_ios(global_ios_box);
    build_ui_android(global_android_box);
    build_ui_common(global_common_box);

    gtk_window_present(GTK_WINDOW(plugin_window));
}
