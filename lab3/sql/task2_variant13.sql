.open data/lab3_task2.db
.headers on
.mode column

.print '=== task 2.3: create table ==='
DROP TABLE IF EXISTS person;
CREATE TABLE person (
    id INTEGER PRIMARY KEY,
    last_name TEXT NOT NULL,
    first_name TEXT NOT NULL,
    patronymic TEXT,
    gender TEXT,
    nationality TEXT,
    height_cm INTEGER,
    weight_kg REAL,
    birth_date TEXT,
    phone TEXT,
    postal_code TEXT,
    country TEXT,
    region TEXT,
    district TEXT,
    city TEXT,
    street TEXT,
    house TEXT,
    apartment TEXT
);

.print '=== task 2.3: insert data ==='
INSERT INTO person VALUES
(1, 'Smirnov', 'Ivan', 'Petrovich', 'M', 'Belarusian', 182, 80.5, '2001-03-11', '+375291110001', '220001', 'Belarus', 'Minsk region', 'Minsk district', 'Minsk', 'Lenina', '10', '12'),
(2, 'Sidorov', 'Pavel', 'Ivanovich', 'M', 'Belarusian', 176, 77.2, '1998-07-22', '+375291110002', '220002', 'Belarus', 'Minsk region', 'Minsk district', 'Minsk', 'Pobedy', '5', '34'),
(3, 'Kovalev', 'Anton', 'Sergeevich', 'M', 'Belarusian', 169, 68.1, '2004-02-01', '+375291110003', '220003', 'Belarus', 'Brest region', 'Brest district', 'Brest', 'Sovetskaya', '17', '6'),
(4, 'Lebedeva', 'Anna', 'Olegovna', 'F', 'Belarusian', 164, 54.6, '2005-11-16', '+375291110004', '220004', 'Belarus', 'Vitebsk region', 'Vitebsk district', 'Vitebsk', 'Mira', '24', '51'),
(5, 'Semenov', 'Nikita', 'Alekseevich', 'M', 'Belarusian', 173, 71.0, '2003-12-03', '+375291110005', '220005', 'Belarus', 'Grodno region', 'Grodno district', 'Grodno', 'Centralnaya', '8', '19'),
(6, 'Ivanova', 'Daria', 'Maksimovna', 'F', 'Belarusian', 171, 60.8, '2002-09-27', '+375291110006', '220006', 'Belarus', 'Minsk region', 'Minsk district', 'Minsk', 'Yanki Kupaly', '42', '7'),
(7, 'Morozov', 'Kirill', 'Dmitrievich', 'M', 'Belarusian', 185, 90.4, '1997-05-15', '+375291110007', '220007', 'Belarus', 'Mogilev region', 'Mogilev district', 'Mogilev', 'Pushkina', '13', '3'),
(8, 'Stepanova', 'Maria', 'Andreevna', 'F', 'Belarusian', 168, 57.3, '2006-01-09', '+375291110008', '220008', 'Belarus', 'Gomel region', 'Gomel district', 'Gomel', 'Molodezhnaya', '77', '22'),
(9, 'Savchenko', 'Artem', 'Romanovich', 'M', 'Belarusian', 179, 74.9, '2000-04-18', '+375291110009', '220009', 'Belarus', 'Minsk region', 'Minsk district', 'Minsk', 'Nezavisimosti', '99', '101'),
(10, 'Orlova', 'Elena', 'Vladimirovna', 'F', 'Belarusian', 166, 59.2, '1999-08-30', '+375291110010', '220010', 'Belarus', 'Brest region', 'Brest district', 'Brest', 'Kirova', '11', '8');

.print '=== task 2.3: select all ==='
SELECT * FROM person;

.print '=== task 2.3: select sorted by id ==='
SELECT id, last_name, first_name, birth_date FROM person ORDER BY id;

.print '=== task 2.3: select sorted by last_name ==='
SELECT id, last_name, first_name, birth_date FROM person ORDER BY last_name;

.print '=== task 2.3: select last 5 rows ==='
SELECT id, last_name, first_name FROM person ORDER BY id DESC LIMIT 5;

.print '=== task 2.3: where id = 5 ==='
SELECT * FROM person WHERE id = 5;

.print '=== task 2.3: where last_name like S% ==='
SELECT id, last_name, first_name FROM person WHERE last_name LIKE 'S%';

.print '=== task 2.3: rename table person -> people ==='
ALTER TABLE person RENAME TO people;

.print '=== task 2.3: update data ==='
UPDATE people
SET phone = '+375291119999', street = 'UpdatedStreet'
WHERE id = 5;
SELECT id, last_name, first_name, phone, street FROM people WHERE id = 5;

.print '=== task 2.4: youngest person ==='
SELECT id, last_name, first_name, birth_date
FROM people
ORDER BY birth_date DESC
LIMIT 1;

.print '=== task 2.4: alter add columns email and city_id ==='
ALTER TABLE people ADD COLUMN email TEXT;
ALTER TABLE people ADD COLUMN city_id INTEGER;

.print '=== task 2.4: create category table for cities ==='
DROP TABLE IF EXISTS category;
CREATE TABLE category (
    id INTEGER PRIMARY KEY,
    city_name TEXT NOT NULL,
    city_description TEXT
);

INSERT INTO category (id, city_name, city_description) VALUES
(1, 'Minsk', 'Capital city'),
(2, 'Brest', 'Regional center in the west'),
(3, 'Vitebsk', 'Regional center in the north'),
(4, 'Grodno', 'Regional center in the west'),
(5, 'Gomel', 'Regional center in the southeast'),
(6, 'Mogilev', 'Regional center in the east');

UPDATE people SET city_id = 1 WHERE id IN (1,2,6,9);
UPDATE people SET city_id = 2 WHERE id IN (3,10);
UPDATE people SET city_id = 3 WHERE id = 4;
UPDATE people SET city_id = 4 WHERE id = 5;
UPDATE people SET city_id = 5 WHERE id = 8;
UPDATE people SET city_id = 6 WHERE id = 7;

.print '=== task 2.4: people with city names ==='
SELECT p.id, p.last_name, p.first_name, p.birth_date, c.city_name
FROM people p
LEFT JOIN category c ON p.city_id = c.id
ORDER BY p.id;

.print '=== task 2.4: count people with height > 170 ==='
SELECT COUNT(*) AS people_count_height_gt_170
FROM people
WHERE height_cm > 170;

.print '=== task 2.4: sum weight where height < 176 ==='
SELECT SUM(weight_kg) AS total_weight_height_lt_176
FROM people
WHERE height_cm < 176;

.print '=== task 2.4: max and min weight ==='
SELECT MAX(weight_kg) AS max_weight, MIN(weight_kg) AS min_weight
FROM people;

.print '=== task 2.4: inner join for city id = 5 ==='
SELECT p.id, p.last_name, p.first_name, c.city_name
FROM people p
INNER JOIN category c ON p.city_id = c.id
WHERE c.id = 5;

.print '=== task 2.3: delete by id ==='
DELETE FROM people WHERE id = 10;
SELECT COUNT(*) AS rows_after_delete_id FROM people;

.print '=== task 2.3: delete by object name (last_name) ==='
DELETE FROM people WHERE last_name = 'Sidorov';
SELECT COUNT(*) AS rows_after_delete_last_name FROM people;

.print '=== task 2.3: export to SQL dump and CSV ==='
.output sql/task2_export.sql
.dump
.output stdout
.mode csv
.once sql/task2_people.csv
SELECT * FROM people;
.mode column

.print '=== task 2.3: drop tables ==='
DROP TABLE people;
DROP TABLE category;

.print '=== done ==='
