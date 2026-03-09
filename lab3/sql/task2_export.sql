PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "people" (
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
, email TEXT, city_id INTEGER);
INSERT INTO people VALUES(1,'Smirnov','Ivan','Petrovich','M','Belarusian',182,80.5,'2001-03-11','+375291110001','220001','Belarus','Minsk region','Minsk district','Minsk','Lenina','10','12',NULL,1);
INSERT INTO people VALUES(3,'Kovalev','Anton','Sergeevich','M','Belarusian',169,68.09999999999999431,'2004-02-01','+375291110003','220003','Belarus','Brest region','Brest district','Brest','Sovetskaya','17','6',NULL,2);
INSERT INTO people VALUES(4,'Lebedeva','Anna','Olegovna','F','Belarusian',164,54.60000000000000142,'2005-11-16','+375291110004','220004','Belarus','Vitebsk region','Vitebsk district','Vitebsk','Mira','24','51',NULL,3);
INSERT INTO people VALUES(5,'Semenov','Nikita','Alekseevich','M','Belarusian',173,71.0,'2003-12-03','+375291119999','220005','Belarus','Grodno region','Grodno district','Grodno','UpdatedStreet','8','19',NULL,4);
INSERT INTO people VALUES(6,'Ivanova','Daria','Maksimovna','F','Belarusian',171,60.79999999999999716,'2002-09-27','+375291110006','220006','Belarus','Minsk region','Minsk district','Minsk','Yanki Kupaly','42','7',NULL,1);
INSERT INTO people VALUES(7,'Morozov','Kirill','Dmitrievich','M','Belarusian',185,90.40000000000000569,'1997-05-15','+375291110007','220007','Belarus','Mogilev region','Mogilev district','Mogilev','Pushkina','13','3',NULL,6);
INSERT INTO people VALUES(8,'Stepanova','Maria','Andreevna','F','Belarusian',168,57.29999999999999716,'2006-01-09','+375291110008','220008','Belarus','Gomel region','Gomel district','Gomel','Molodezhnaya','77','22',NULL,5);
INSERT INTO people VALUES(9,'Savchenko','Artem','Romanovich','M','Belarusian',179,74.90000000000000569,'2000-04-18','+375291110009','220009','Belarus','Minsk region','Minsk district','Minsk','Nezavisimosti','99','101',NULL,1);
CREATE TABLE category (
    id INTEGER PRIMARY KEY,
    city_name TEXT NOT NULL,
    city_description TEXT
);
INSERT INTO category VALUES(1,'Minsk','Capital city');
INSERT INTO category VALUES(2,'Brest','Regional center in the west');
INSERT INTO category VALUES(3,'Vitebsk','Regional center in the north');
INSERT INTO category VALUES(4,'Grodno','Regional center in the west');
INSERT INTO category VALUES(5,'Gomel','Regional center in the southeast');
INSERT INTO category VALUES(6,'Mogilev','Regional center in the east');
COMMIT;
