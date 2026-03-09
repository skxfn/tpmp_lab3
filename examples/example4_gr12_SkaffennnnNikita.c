#include <sqlite3.h>
#include <stdio.h>

int main(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("task4_examples.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_exec(db,
        "DROP TABLE IF EXISTS payments_demo;"
        "CREATE TABLE payments_demo(id INTEGER PRIMARY KEY, note TEXT, amount REAL);",
        NULL, NULL, NULL);

    rc = sqlite3_exec(db,
        "INSERT INTO payments_demo(note, amount) VALUES('autocommit_1', 10.0);"
        "INSERT INTO payments_demo(note, amount) VALUES('autocommit_2', 20.0);",
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Autocommit inserts failed\n");
    }

    rc = sqlite3_exec(db,
        "BEGIN TRANSACTION;"
        "INSERT INTO payments_demo(note, amount) VALUES('tx_1', 100.0);"
        "INSERT INTO payments_demo(note, amount) VALUES('tx_2', 200.0);"
        "INSERT INTO payments_demo(note, amount) VALUES('tx_3', 300.0);"
        "COMMIT;",
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Transaction inserts failed\n");
    }

    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*), SUM(amount) FROM payments_demo;", -1, &stmt, NULL);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        printf("example4: rows=%d sum=%.2f\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_double(stmt, 1));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
