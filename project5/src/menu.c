#include "menu.h"

#include "db.h"
#include "person.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_menu(void) {
    printf("\n=== Project5 Menu ===\n");
    printf("1. Init schema + seed cities\n");
    printf("2. Insert one demo person (autocommit)\n");
    printf("3. Insert three demo people (transaction)\n");
    printf("4. Select all\n");
    printf("5. Select by id\n");
    printf("6. Select by last_name pattern\n");
    printf("7. Select by city\n");
    printf("8. Delete by id\n");
    printf("9. Attach photo (BLOB) by id\n");
    printf("10. Export photo (BLOB) by id\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

static int read_int(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) {
        return -1;
    }
    return atoi(buf);
}

static void read_line(char *dst, size_t size) {
    if (fgets(dst, (int)size, stdin)) {
        dst[strcspn(dst, "\n")] = '\0';
    }
}

static Person make_demo_person_a(void) {
    Person p = {
        "Smirnov", "Ivan", "Petrovich", "M", "Belarusian",
        182, 80.5, "2001-03-11", "+375291000001", "220001",
        "Belarus", "Minsk region", "Minsk district", "Minsk", "Lenina",
        "10", "12", 1
    };
    return p;
}

static Person make_demo_person_b(void) {
    Person p = {
        "Kovalev", "Anton", "Sergeevich", "M", "Belarusian",
        169, 68.1, "2004-02-01", "+375291000002", "220002",
        "Belarus", "Brest region", "Brest district", "Brest", "Sovetskaya",
        "17", "6", 2
    };
    return p;
}

static Person make_demo_person_c(void) {
    Person p = {
        "Stepanova", "Maria", "Andreevna", "F", "Belarusian",
        168, 57.3, "2006-01-09", "+375291000003", "220003",
        "Belarus", "Gomel region", "Gomel district", "Gomel", "Molodezhnaya",
        "77", "22", 5
    };
    return p;
}

int run_app(void) {
    sqlite3 *db = NULL;
    int running = 1;

    if (db_open(&db, "data/project5_people.db") != 0) {
        return 1;
    }

    while (running) {
        int choice;
        print_menu();
        choice = read_int();

        switch (choice) {
            case 1:
                db_init_schema(db);
                db_seed_cities(db);
                printf("Schema and cities ready\n");
                break;
            case 2: {
                Person p = make_demo_person_a();
                db_add_person_autocommit(db, &p);
                break;
            }
            case 3: {
                Person list[3];
                list[0] = make_demo_person_a();
                list[1] = make_demo_person_b();
                list[2] = make_demo_person_c();
                db_add_people_transaction(db, list, 3);
                break;
            }
            case 4:
                db_print_all_people(db);
                break;
            case 5: {
                int id;
                printf("Enter id: ");
                id = read_int();
                db_find_person_by_id(db, id);
                break;
            }
            case 6: {
                char pattern[64];
                printf("Enter pattern (example Sm%%): ");
                read_line(pattern, sizeof(pattern));
                db_find_people_by_lastname_pattern(db, pattern);
                break;
            }
            case 7: {
                char city[64];
                printf("Enter city name: ");
                read_line(city, sizeof(city));
                db_find_people_by_city(db, city);
                break;
            }
            case 8: {
                int id;
                printf("Enter id to delete: ");
                id = read_int();
                db_delete_person(db, id);
                break;
            }
            case 9: {
                int id;
                char path[256];
                printf("Enter id to attach photo: ");
                id = read_int();
                printf("Enter input file path: ");
                read_line(path, sizeof(path));
                db_attach_photo(db, id, path);
                break;
            }
            case 10: {
                int id;
                char path[256];
                printf("Enter id to export photo: ");
                id = read_int();
                printf("Enter output file path: ");
                read_line(path, sizeof(path));
                db_export_photo(db, id, path);
                break;
            }
            case 0:
                running = 0;
                break;
            default:
                printf("Unknown menu item\n");
                break;
        }
    }

    db_close(db);
    return 0;
}
