#ifndef DATABASE_H
#define DATABASE_H

/* Initialize SQLite database and create saved_devices table if it doesn't exist */
int init_database(void);

/* Register a newly connected device into the database safely */
void register_new_device(const char *vid, const char *serial, const char *brand, const char *model, const char *custom_name);

/* Load all registered devices from the database using a custom UI callback function */
void load_saved_devices(void (*callback)(const char *vid, const char *brand, const char *model, const char *custom_name));

#endif /* DATABASE_H */
