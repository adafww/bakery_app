#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("470211");

    QSqlDatabase tempDb = QSqlDatabase::addDatabase("QMYSQL", "temp_connection");
    tempDb.setHostName("127.0.0.1");
    tempDb.setPort(3306);
    tempDb.setUserName("root");
    tempDb.setPassword("470211");

    if (!tempDb.open()) {
        qDebug() << "Ошибка подключения к MariaDB:" << tempDb.lastError().text();
        return;
    }

    QSqlQuery query(tempDb);
    query.exec("CREATE DATABASE IF NOT EXISTS bakery_db;");
    tempDb.close();

    db.setDatabaseName("bakery_db");

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return;
    }

    qDebug() << "Подключение к базе данных успешно!";
    createTables();
    insertTestData();
}

void DatabaseManager::createTables() {
    QSqlQuery query;

    // Создаем базу данных, если её нет
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта!";
        return;
    }


    query.exec("CREATE TABLE IF NOT EXISTS products (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), price DECIMAL(10,2), quantity INT)");
    query.exec("CREATE TABLE IF NOT EXISTS orders (id INT AUTO_INCREMENT PRIMARY KEY, product_id INT, quantity INT, order_date DATE, FOREIGN KEY (product_id) REFERENCES products(id))");
    query.exec("CREATE TABLE IF NOT EXISTS employees (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), position VARCHAR(255), salary DECIMAL(10,2))");
    query.exec("CREATE TABLE IF NOT EXISTS production (id INT AUTO_INCREMENT PRIMARY KEY, product_id INT, production_date DATE, quantity INT, FOREIGN KEY (product_id) REFERENCES products(id))");
    query.exec("CREATE TABLE IF NOT EXISTS sales (id INT AUTO_INCREMENT PRIMARY KEY, order_id INT, sale_date DATE, total_amount DECIMAL(10,2), FOREIGN KEY (order_id) REFERENCES orders(id))");

    if (query.lastError().isValid()) {
        qDebug() << "Ошибка при создании таблиц:" << query.lastError().text();
    } else {
        qDebug() << "Таблицы успешно созданы или уже существуют.";
    }
}

void DatabaseManager::insertTestData() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM products");
    if (query.next() && query.value(0).toInt() > 0) return;

    query.exec("INSERT INTO products (name, price, quantity) VALUES ('Хлеб', 30.00, 100), ('Булочка', 15.00, 200), ('Кекс', 40.00, 50)");
    query.exec("INSERT INTO employees (name, position, salary) VALUES ('Иван Иванов', 'Пекарь', 35000), ('Мария Петрова', 'Продавец', 25000)");
    query.exec("INSERT INTO orders (product_id, quantity, order_date) VALUES (1, 50, '2025-03-27'), (2, 100, '2025-03-27')");
    query.exec("INSERT INTO production (product_id, production_date, quantity) VALUES (1, '2025-03-27', 200), (2, '2025-03-27', 150)");
    query.exec("INSERT INTO sales (order_id, sale_date, total_amount) VALUES (1, '2025-03-27', 1500.00), (2, '2025-03-27', 1500.00)");
}
