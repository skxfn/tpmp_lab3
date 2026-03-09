#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include "person.h"

int db_open(sqlite3 **db, const char *path);
void db_close(sqlite3 *db);
int db_init_schema(sqlite3 *db);
int db_seed_cities(sqlite3 *db);
int db_add_person_autocommit(sqlite3 *db, const Person *p);
int db_add_people_transaction(sqlite3 *db, const Person *people, int count);
int db_print_all_people(sqlite3 *db);
int db_find_person_by_id(sqlite3 *db, int id);
int db_find_people_by_lastname_pattern(sqlite3 *db, const char *pattern);
int db_find_people_by_city(sqlite3 *db, const char *city_name);
int db_delete_person(sqlite3 *db, int id);
int db_attach_photo(sqlite3 *db, int person_id, const char *input_path);
int db_export_photo(sqlite3 *db, int person_id, const char *output_path);

#endif
