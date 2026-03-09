#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_sqlite_error(sqlite3 *db, const char *context) {
    fprintf(stderr, "%s: %s\n", context, sqlite3_errmsg(db));
}

static void print_person_row(sqlite3_stmt *stmt) {
    printf("id=%d | %s %s | birth=%s | city=%s | phone=%s\n",
           sqlite3_column_int(stmt, 0),
           sqlite3_column_text(stmt, 1),
           sqlite3_column_text(stmt, 2),
           sqlite3_column_text(stmt, 3),
           sqlite3_column_text(stmt, 4),
           sqlite3_column_text(stmt, 5));
}

int db_open(sqlite3 **db, const char *path) {
    if (sqlite3_open(path, db) != SQLITE_OK) {
        print_sqlite_error(*db, "Failed to open database");
        return 1;
    }
    return 0;
}

void db_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int db_init_schema(sqlite3 *db) {
    const char *sql =
        "PRAGMA foreign_keys = ON;"
        "CREATE TABLE IF NOT EXISTS cities("
        "  id INTEGER PRIMARY KEY,"
        "  name TEXT NOT NULL UNIQUE,"
        "  description TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS people("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  last_name TEXT NOT NULL,"
        "  first_name TEXT NOT NULL,"
        "  patronymic TEXT,"
        "  gender TEXT,"
        "  nationality TEXT,"
        "  height_cm INTEGER,"
        "  weight_kg REAL,"
        "  birth_date TEXT,"
        "  phone TEXT,"
        "  postal_code TEXT,"
        "  country TEXT,"
        "  region TEXT,"
        "  district TEXT,"
        "  city TEXT,"
        "  street TEXT,"
        "  house TEXT,"
        "  apartment TEXT,"
        "  city_id INTEGER,"
        "  photo BLOB,"
        "  FOREIGN KEY(city_id) REFERENCES cities(id)"
        ");";

    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Schema init failed");
        return 1;
    }
    return 0;
}

int db_seed_cities(sqlite3 *db) {
    const char *sql =
        "INSERT OR IGNORE INTO cities(id, name, description) VALUES"
        "(1, 'Minsk', 'Capital city'),"
        "(2, 'Brest', 'Regional center'),"
        "(3, 'Vitebsk', 'Regional center'),"
        "(4, 'Grodno', 'Regional center'),"
        "(5, 'Gomel', 'Regional center'),"
        "(6, 'Mogilev', 'Regional center');";

    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "City seed failed");
        return 1;
    }
    return 0;
}

static int bind_person(sqlite3_stmt *stmt, const Person *p) {
    sqlite3_bind_text(stmt, 1, p->last_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, p->first_name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, p->patronymic, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, p->gender, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, p->nationality, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, p->height_cm);
    sqlite3_bind_double(stmt, 7, p->weight_kg);
    sqlite3_bind_text(stmt, 8, p->birth_date, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, p->phone, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, p->postal_code, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11, p->country, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, p->region, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 13, p->district, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 14, p->city, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 15, p->street, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 16, p->house, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 17, p->apartment, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 18, p->city_id);
    return 0;
}

int db_add_person_autocommit(sqlite3 *db, const Person *p) {
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "INSERT INTO people("
        "last_name, first_name, patronymic, gender, nationality,"
        "height_cm, weight_kg, birth_date, phone, postal_code, country,"
        "region, district, city, street, house, apartment, city_id"
        ") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare insert failed");
        return 1;
    }

    bind_person(stmt, p);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        print_sqlite_error(db, "Autocommit insert failed");
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    printf("Inserted 1 row (autocommit)\n");
    return 0;
}

int db_add_people_transaction(sqlite3 *db, const Person *people, int count) {
    sqlite3_stmt *stmt = NULL;
    int i;
    const char *sql =
        "INSERT INTO people("
        "last_name, first_name, patronymic, gender, nationality,"
        "height_cm, weight_kg, birth_date, phone, postal_code, country,"
        "region, district, city, street, house, apartment, city_id"
        ") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "BEGIN failed");
        return 1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare transaction insert failed");
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 1;
    }

    for (i = 0; i < count; i++) {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        bind_person(stmt, &people[i]);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            print_sqlite_error(db, "Transaction insert row failed");
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
            return 1;
        }
    }

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "COMMIT failed");
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return 1;
    }

    printf("Inserted %d rows (transaction)\n", count);
    return 0;
}

int db_print_all_people(sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT p.id, p.last_name, p.first_name, p.birth_date, c.name, p.phone "
        "FROM people p "
        "LEFT JOIN cities c ON c.id = p.city_id "
        "ORDER BY p.id;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare select all failed");
        return 1;
    }

    printf("All people:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        print_person_row(stmt);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_find_person_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT p.id, p.last_name, p.first_name, p.birth_date, c.name, p.phone "
        "FROM people p "
        "LEFT JOIN cities c ON c.id = p.city_id "
        "WHERE p.id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare select by id failed");
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        print_person_row(stmt);
    } else {
        printf("No person with id=%d\n", id);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_find_people_by_lastname_pattern(sqlite3 *db, const char *pattern) {
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT p.id, p.last_name, p.first_name, p.birth_date, c.name, p.phone "
        "FROM people p "
        "LEFT JOIN cities c ON c.id = p.city_id "
        "WHERE p.last_name LIKE ? "
        "ORDER BY p.last_name;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare select by pattern failed");
        return 1;
    }

    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_TRANSIENT);
    printf("Pattern '%s':\n", pattern);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        print_person_row(stmt);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_find_people_by_city(sqlite3 *db, const char *city_name) {
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT p.id, p.last_name, p.first_name, p.birth_date, c.name, p.phone "
        "FROM people p "
        "JOIN cities c ON c.id = p.city_id "
        "WHERE c.name = ? "
        "ORDER BY p.id;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare select by city failed");
        return 1;
    }

    sqlite3_bind_text(stmt, 1, city_name, -1, SQLITE_TRANSIENT);
    printf("City '%s':\n", city_name);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        print_person_row(stmt);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_delete_person(sqlite3 *db, int id) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "DELETE FROM people WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare delete failed");
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        print_sqlite_error(db, "Delete failed");
        sqlite3_finalize(stmt);
        return 1;
    }

    printf("Deleted rows: %d\n", sqlite3_changes(db));
    sqlite3_finalize(stmt);
    return 0;
}

static int read_binary(const char *path, unsigned char **data, int *size) {
    FILE *f = fopen(path, "rb");
    long len;
    if (!f) {
        return 1;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 1;
    }
    len = ftell(f);
    if (len < 0) {
        fclose(f);
        return 1;
    }
    rewind(f);

    *data = (unsigned char *)malloc((size_t)len);
    if (!*data) {
        fclose(f);
        return 1;
    }

    if (fread(*data, 1, (size_t)len, f) != (size_t)len) {
        free(*data);
        fclose(f);
        return 1;
    }

    fclose(f);
    *size = (int)len;
    return 0;
}

static int write_binary(const char *path, const unsigned char *data, int size) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        return 1;
    }
    if (fwrite(data, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

int db_attach_photo(sqlite3 *db, int person_id, const char *input_path) {
    sqlite3_stmt *stmt = NULL;
    unsigned char *blob = NULL;
    int blob_size = 0;
    const char *sql = "UPDATE people SET photo = ? WHERE id = ?;";

    if (read_binary(input_path, &blob, &blob_size) != 0) {
        fprintf(stderr, "Cannot read photo file: %s\n", input_path);
        return 1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare attach photo failed");
        free(blob);
        return 1;
    }

    sqlite3_bind_blob(stmt, 1, blob, blob_size, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, person_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        print_sqlite_error(db, "Attach photo failed");
        sqlite3_finalize(stmt);
        free(blob);
        return 1;
    }

    printf("Photo attached for id=%d, bytes=%d\n", person_id, blob_size);
    sqlite3_finalize(stmt);
    free(blob);
    return 0;
}

int db_export_photo(sqlite3 *db, int person_id, const char *output_path) {
    sqlite3_stmt *stmt = NULL;
    const void *blob;
    int blob_size;
    const char *sql = "SELECT photo FROM people WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        print_sqlite_error(db, "Prepare export photo failed");
        return 1;
    }

    sqlite3_bind_int(stmt, 1, person_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("No person or photo for id=%d\n", person_id);
        sqlite3_finalize(stmt);
        return 1;
    }

    blob = sqlite3_column_blob(stmt, 0);
    blob_size = sqlite3_column_bytes(stmt, 0);
    if (!blob || blob_size <= 0) {
        printf("Photo is empty for id=%d\n", person_id);
        sqlite3_finalize(stmt);
        return 1;
    }

    if (write_binary(output_path, (const unsigned char *)blob, blob_size) != 0) {
        fprintf(stderr, "Failed to write output photo: %s\n", output_path);
        sqlite3_finalize(stmt);
        return 1;
    }

    printf("Photo exported for id=%d to %s, bytes=%d\n", person_id, output_path, blob_size);
    sqlite3_finalize(stmt);
    return 0;
}
