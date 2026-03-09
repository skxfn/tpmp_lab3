#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

static int read_file(const char *path, unsigned char **buf, int *size) {
    FILE *f = fopen(path, "rb");
    long len;
    if (!f) {
        return 0;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }
    len = ftell(f);
    if (len < 0) {
        fclose(f);
        return 0;
    }
    rewind(f);
    *buf = (unsigned char *)malloc((size_t)len);
    if (!*buf) {
        fclose(f);
        return 0;
    }
    if (fread(*buf, 1, (size_t)len, f) != (size_t)len) {
        free(*buf);
        fclose(f);
        return 0;
    }
    fclose(f);
    *size = (int)len;
    return 1;
}

static int write_file(const char *path, const unsigned char *buf, int size) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        return 0;
    }
    if (fwrite(buf, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

int main(void) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    unsigned char *file_buf = NULL;
    int file_size = 0;
    int rc;

    rc = sqlite3_open("task4_examples.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_exec(db,
        "DROP TABLE IF EXISTS photos_demo;"
        "CREATE TABLE photos_demo(id INTEGER PRIMARY KEY, name TEXT, photo BLOB);",
        NULL, NULL, NULL);

    if (!read_file("sample_photo.bin", &file_buf, &file_size)) {
        fprintf(stderr, "Cannot read sample_photo.bin\n");
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_prepare_v2(db, "INSERT INTO photos_demo(name, photo) VALUES(?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare insert failed: %s\n", sqlite3_errmsg(db));
        free(file_buf);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, "sample", -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, file_buf, file_size, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Blob insert failed: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    free(file_buf);

    rc = sqlite3_prepare_v2(db, "SELECT photo FROM photos_demo WHERE id=1;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare select failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const void *blob = sqlite3_column_blob(stmt, 0);
        int blob_size = sqlite3_column_bytes(stmt, 0);
        if (!write_file("exported_photo.bin", (const unsigned char *)blob, blob_size)) {
            fprintf(stderr, "Write exported blob failed\n");
        } else {
            printf("example3: blob bytes written=%d\n", blob_size);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
