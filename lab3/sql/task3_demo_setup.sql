.open data/expenses_demo.db

DROP TABLE IF EXISTS Categories;
DROP TABLE IF EXISTS Spendings;
DROP TABLE IF EXISTS Goods;
DROP TABLE IF EXISTS Goods_Spendings;

CREATE TABLE Categories (
    id INTEGER PRIMARY KEY,
    category_name TEXT NOT NULL
);

CREATE TABLE Spendings (
    id INTEGER PRIMARY KEY,
    category_id INTEGER NOT NULL,
    shop_name TEXT NOT NULL,
    amount REAL NOT NULL,
    spending_date TEXT,
    FOREIGN KEY (category_id) REFERENCES Categories(id)
);

CREATE TABLE Goods (
    id INTEGER PRIMARY KEY,
    goods_name TEXT NOT NULL
);

CREATE TABLE Goods_Spendings (
    id INTEGER PRIMARY KEY,
    goods_id INTEGER NOT NULL,
    amount REAL NOT NULL,
    FOREIGN KEY (goods_id) REFERENCES Goods(id)
);

INSERT INTO Categories (id, category_name) VALUES
(1, 'Food'),
(2, 'Transport'),
(3, 'Health'),
(4, 'Home'),
(5, 'Electronics');

INSERT INTO Spendings (id, category_id, shop_name, amount, spending_date) VALUES
(1, 5, 'TechnoMarket', 899.99, '2026-03-01'),
(2, 5, 'GadgetCity', 499.50, '2026-03-02'),
(3, 1, 'GreenStore', 45.10, '2026-03-03'),
(4, 2, 'Metro', 18.00, '2026-03-03');

INSERT INTO Goods (id, goods_name) VALUES
(1, 'Smartphone'),
(2, 'Laptop Bag'),
(3, 'Headphones'),
(4, 'Monitor');

INSERT INTO Goods_Spendings (id, goods_id, amount) VALUES
(1, 1, 1200.00),
(2, 2, 150.00),
(3, 3, 450.00),
(4, 4, 600.00);
