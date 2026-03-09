#include <sqlite3.h>
#include <stdio.h>

static int table_callback(void *unused, int argc, char **argv, char **col_names) {
    int i;
    (void)unused;
    for (i = 0; i < argc; i++) {
        printf("%s=%s%s", col_names[i], argv[i] ? argv[i] : "NULL", (i + 1 == argc) ? "\n" : " | ");
    }
    return 0;
}

int main(void) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_open("task4_examples.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    printf("example5: database metadata\n");
    printf("sqlite version: %s\n", sqlite3_libversion());

    rc = sqlite3_exec(db,
        "SELECT name, type FROM sqlite_master WHERE type IN ('table','view') ORDER BY name;",
        table_callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Metadata query failed\n");
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_prepare_v2(db, "PRAGMA table_info(demo_people);", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        printf("Columns in demo_people:\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("cid=%d | name=%s | type=%s\n",
                   sqlite3_column_int(stmt, 0),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_text(stmt, 2));
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
