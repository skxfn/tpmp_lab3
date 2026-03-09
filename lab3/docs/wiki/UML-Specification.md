# UML спецификация приложения `project5`

## 1. Назначение
Консольное приложение на C для работы с SQLite БД по сущности «Человек» (вариант 13):
- инициализация схемы БД;
- CRUD-операции с таблицей `people`;
- выборки по `id`, шаблону фамилии и городу;
- демонстрация вставки в режимах autocommit и transaction;
- работа с фотографией (BLOB): запись в БД и выгрузка в файл.

## 2. Диаграмма вариантов использования (Use Case)

```mermaid
flowchart LR
    U[Пользователь]
    APP[Консольное приложение project5]

    U --> UC1[Инициализировать БД]
    U --> UC2[Добавить человека\n(autocommit)]
    U --> UC3[Добавить людей\n(transaction)]
    U --> UC4[Показать всех людей]
    U --> UC5[Поиск по id]
    U --> UC6[Поиск по шаблону фамилии]
    U --> UC7[Поиск по городу]
    U --> UC8[Удалить по id]
    U --> UC9[Прикрепить фото (BLOB)]
    U --> UC10[Экспортировать фото (BLOB)]

    UC1 --> APP
    UC2 --> APP
    UC3 --> APP
    UC4 --> APP
    UC5 --> APP
    UC6 --> APP
    UC7 --> APP
    UC8 --> APP
    UC9 --> APP
    UC10 --> APP
```

## 3. Диаграмма классов/компонентов

```mermaid
classDiagram
    class main_c {
        +main(): int
    }

    class menu_c {
        +run_app(): int
        -print_menu(): void
        -read_int(): int
        -read_line(dst,size): void
    }

    class db_c {
        +db_open(db,path): int
        +db_close(db): void
        +db_init_schema(db): int
        +db_seed_cities(db): int
        +db_add_person_autocommit(db,p): int
        +db_add_people_transaction(db,people,count): int
        +db_print_all_people(db): int
        +db_find_person_by_id(db,id): int
        +db_find_people_by_lastname_pattern(db,pattern): int
        +db_find_people_by_city(db,city): int
        +db_delete_person(db,id): int
        +db_attach_photo(db,id,input_path): int
        +db_export_photo(db,id,output_path): int
    }

    class Person {
        +last_name: char[128]
        +first_name: char[128]
        +patronymic: char[128]
        +gender: char[16]
        +nationality: char[64]
        +height_cm: int
        +weight_kg: double
        +birth_date: char[16]
        +phone: char[32]
        +postal_code: char[16]
        +country: char[64]
        +region: char[64]
        +district: char[64]
        +city: char[64]
        +street: char[64]
        +house: char[16]
        +apartment: char[16]
        +city_id: int
    }

    class SQLite_DB {
        +cities(id,name,description)
        +people(..., city_id, photo)
    }

    main_c --> menu_c : вызывает
    menu_c --> db_c : операции БД
    db_c --> Person : использует
    db_c --> SQLite_DB : SQL-запросы
```

## 4. Диаграмма последовательностей (пример: поиск по id)

```mermaid
sequenceDiagram
    actor User as Пользователь
    participant Menu as menu.c
    participant DB as db.c
    participant SQL as SQLite

    User->>Menu: Выбор пункта 5 + ввод id
    Menu->>DB: db_find_person_by_id(db, id)
    DB->>SQL: prepare/select + bind(id)
    SQL-->>DB: result row / no row
    DB-->>Menu: печать результата
    Menu-->>User: запись найдена / не найдена
```

## 5. Диаграмма активности (общий сценарий)

```mermaid
flowchart TD
    A([Старт]) --> B[Открытие SQLite БД]
    B --> C{Выбор пункта меню}
    C -->|1| D[Init schema + seed cities]
    C -->|2| E[Insert person autocommit]
    C -->|3| F[Insert 3 persons transaction]
    C -->|4| G[Select all]
    C -->|5| H[Select by id]
    C -->|6| I[Select by pattern]
    C -->|7| J[Select by city]
    C -->|8| K[Delete by id]
    C -->|9| L[Attach photo BLOB]
    C -->|10| M[Export photo BLOB]
    C -->|0| N[Close DB]

    D --> C
    E --> C
    F --> C
    G --> C
    H --> C
    I --> C
    J --> C
    K --> C
    L --> C
    M --> C
    N --> O([Завершение])
```

## 6. Трассировка с кодом
- Интерфейс приложения: `lab3/project5/src/menu.c`
- Точка входа: `lab3/project5/src/main.c`
- Логика доступа к БД: `lab3/project5/src/db.c`
- Модель данных: `lab3/project5/include/person.h`
