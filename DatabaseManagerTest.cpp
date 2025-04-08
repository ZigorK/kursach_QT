#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Создаем подключение к базе данных SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");

    // Проверяем подключение
    if (!db.open()) {
        qDebug() << "Error: " << db.lastError().text();
        return 1;
    }

    qDebug() << "Connected to SQLite database successfully!";

    // Создаем таблицу, если ее нет
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)")) {
        qDebug() << "Error creating table: " << query.lastError().text();
        return 1;
    }

    qDebug() << "Table created successfully!";

    // Вставляем данные
    query.prepare("INSERT INTO users (name, age) VALUES (:name, :age)");
    query.bindValue(":name", "John Doe");
    query.bindValue(":age", 30);

    if (!query.exec()) {
        qDebug() << "Error inserting data: " << query.lastError().text();
        return 1;
    }

    qDebug() << "Data inserted successfully!";

    // Читаем и выводим данные
    if (query.exec("SELECT * FROM users")) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            int age = query.value(2).toInt();
            qDebug() << "ID:" << id << ", Name:" << name << ", Age:" << age;
        }
    } else {
        qDebug() << "Error reading data: " << query.lastError().text();
        return 1;
    }

    return a.exec();
}
