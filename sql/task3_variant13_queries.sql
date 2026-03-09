-- Variant 13, task 3.2

-- 1) Category name, shop name and amount for spendings in category id=5
SELECT c.category_name,
       s.shop_name,
       s.amount
FROM Categories c
JOIN Spendings s ON s.category_id = c.id
WHERE c.id = 5;

-- 2) Goods names and spending amounts greater than 400 RUB
SELECT g.goods_name,
       gs.amount
FROM Goods g
JOIN Goods_Spendings gs ON gs.goods_id = g.id
WHERE gs.amount > 400;
