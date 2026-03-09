#include <sqlite3.h>
#include <stdio.h>

int main(void) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_open("task4_examples.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db,
        "DROP TABLE IF EXISTS products;"
        "CREATE TABLE products(id INTEGER PRIMARY KEY, name TEXT, price REAL);",
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Init SQL failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_prepare_v2(db, "INSERT INTO products(name, price) VALUES(?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, "Laptop", -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, 1999.90);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Insert 1 failed\n");
    }
    sqlite3_reset(stmt);

    sqlite3_bind_text(stmt, 1, "Mouse", -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, 59.50);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Insert 2 failed\n");
    }
    sqlite3_finalize(stmt);

    rc = sqlite3_prepare_v2(db, "SELECT id, name, price FROM products WHERE price > ? ORDER BY price DESC;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare select failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_double(stmt, 1, 100.0);

    printf("example2: products with price > 100\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("id=%d | name=%s | price=%.2f\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_double(stmt, 2));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
