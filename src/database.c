#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

int init_database() {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("projectphone.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[БД Ошибка]: Не удалось открыть файл БД: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // ВЗРОСЛАЯ СТРУКТУРА: Разносим всё по отдельным колонкам на перспективу
    const char *sql = "CREATE TABLE IF NOT EXISTS saved_devices ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "vendor_id TEXT, "
                      "serial_number TEXT UNIQUE, " // Главный уникальный ключ телефона
                      "brand TEXT, "
                      "model TEXT, "
                      "custom_name TEXT, "
                      "is_connected INTEGER DEFAULT 0);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[БД Ошибка]: Не удалось создать таблицу: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}

// Умная функция регистрации: принимает все параметры раздельно
void register_new_device(const char *vid, const char *serial, const char *brand, const char *model, const char *custom_name) {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    if (sqlite3_open("projectphone.db", &db) != SQLITE_OK) return;

    // Проверяем уникальность по серийному номеру (UDID)
    const char *check_sql = "SELECT id FROM saved_devices WHERE serial_number = ?;";
    if (sqlite3_prepare_v2(db, check_sql, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_text(res, 1, serial, -1, SQLITE_STATIC);
        if (sqlite3_step(res) == SQLITE_ROW) {
            sqlite3_finalize(res);
            sqlite3_close(db);
            return; // Уже есть в базе
        }
        sqlite3_finalize(res);
    }

    // Безопасная вставка новой записи
    const char *insert_sql = "INSERT INTO saved_devices (vendor_id, serial_number, brand, model, custom_name, is_connected) VALUES (?, ?, ?, ?, ?, 0);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_text(res, 1, vid, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, serial, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 3, brand, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 4, model, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 5, custom_name, -1, SQLITE_STATIC);
        sqlite3_step(res);
        sqlite3_finalize(res);
        printf("[БД Система]: Привязано новое устройство: %s %s (%s)\n", brand, model, custom_name);
    }

    sqlite3_close(db);
}

// Загрузка устройств: теперь считываем бренд, модель и имя
void load_saved_devices(void (*callback)(const char *vid, const char *brand, const char *model, const char *custom_name)) {
    sqlite3 *db;
    sqlite3_stmt *res;

    if (sqlite3_open("projectphone.db", &db) != SQLITE_OK) return;

    const char *sql = "SELECT vendor_id, brand, model, custom_name FROM saved_devices;";
    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            const char *vid = (const char *)sqlite3_column_text(res, 0);
            const char *brand = (const char *)sqlite3_column_text(res, 1);
            const char *model = (const char *)sqlite3_column_text(res, 2);
            const char *c_name = (const char *)sqlite3_column_text(res, 3);
            if (vid && brand && model && c_name) {
                callback(vid, brand, model, c_name);
            }
        }
        sqlite3_finalize(res);
    }
    sqlite3_close(db);
}
