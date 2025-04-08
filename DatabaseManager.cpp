#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {
    model = new QSqlQueryModel();
}

DatabaseManager::~DatabaseManager() {
    closeConnection();
    delete model;
}

bool DatabaseManager::connectToDatabase(const QString &dbType, const QString &host, const QString &dbName,
                                        const QString &user, const QString &password) {
    db = QSqlDatabase::addDatabase(dbType);

    if (dbType == "QSQLITE") {
        db.setDatabaseName(dbName);  // Для SQLite используется путь к файлу
    } else if (dbType == "QPSQL") {
        db.setHostName(host);
        db.setDatabaseName(dbName);
        db.setUserName(user);
        db.setPassword(password);
    }

    if (db.open()) {
        lastError.clear();
        return true;
    } else {
        lastError = db.lastError().text();
        return false;
    }
}

QString DatabaseManager::getLastError() const {
    return lastError;
}

QSqlQueryModel* DatabaseManager::getModel() {
    return model;
}

void DatabaseManager::closeConnection() {
    if (db.isOpen()) {
        db.close();
    }
}
