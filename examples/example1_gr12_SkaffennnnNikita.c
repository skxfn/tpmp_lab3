#include <sqlite3.h>
#include <stdio.h>

static int print_callback(void *unused, int argc, char **argv, char **col_names) {
    int i;
    (void)unused;
    for (i = 0; i < argc; i++) {
        printf("%s=%s%s", col_names[i], argv[i] ? argv[i] : "NULL", (i + 1 == argc) ? "\n" : " | ");
    }
    return 0;
}

int main(void) {
    sqlite3 *db = NULL;
    char *err = NULL;
    int rc = sqlite3_open("task4_examples.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS demo_people;"
        "CREATE TABLE demo_people(id INTEGER PRIMARY KEY, name TEXT, age INTEGER);"
        "INSERT INTO demo_people(name, age) VALUES('Ivan', 22), ('Anna', 21), ('Pavel', 24);",
        NULL, NULL, &err);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    printf("example1: rows in demo_people\n");
    rc = sqlite3_exec(db, "SELECT id, name, age FROM demo_people ORDER BY id;", print_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
