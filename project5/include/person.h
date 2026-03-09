#ifndef PERSON_H
#define PERSON_H

#define TEXT_LEN 128

typedef struct {
    char last_name[TEXT_LEN];
    char first_name[TEXT_LEN];
    char patronymic[TEXT_LEN];
    char gender[16];
    char nationality[64];
    int height_cm;
    double weight_kg;
    char birth_date[16];
    char phone[32];
    char postal_code[16];
    char country[64];
    char region[64];
    char district[64];
    char city[64];
    char street[64];
    char house[16];
    char apartment[16];
    int city_id;
} Person;

#endif
