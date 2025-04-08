#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QSqlQueryModel>
#include <QMessageBox>

class DatabaseManager {
public:
    // Конструктор
    DatabaseManager();
    
    // Деструктор
    ~DatabaseManager();
    
    // Функция для подключения к базе данных
    bool connectToDatabase(const QString &dbType, const QString &host, const QString &dbName,
                        const QString &user, const QString &password);
    
    // Получить ошибку подключения
    QString getLastError() const;
    
    // Получить модель для работы с результатами запросов
    QSqlQueryModel* getModel();
    
    // Закрытие соединения
    void closeConnection();

private:
    QSqlDatabase db;
    QSqlQueryModel *model;
    QString lastError;
};

#endif // DATABASEMANAGER_H
