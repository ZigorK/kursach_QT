#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), dbManager(new DatabaseManager()) {
    ui->setupUi(this);
    ui->tableView->setModel(dbManager->getModel());
    ui->dbTypeCombo->addItem("QSQLITE");  // Добавляем SQLite в список
    ui->dbTypeCombo->addItem("QPSQL");    // Добавляем PostgreSQL в список
}

MainWindow::~MainWindow() {
    delete ui;
    delete dbManager;
}

void MainWindow::on_connectButton_clicked() {
    QString dbType = ui->dbTypeCombo->currentText();  // Получаем тип базы данных
    QString host = ui->hostEdit->text();
    QString dbName = ui->dbNameEdit->text();
    QString user = ui->userEdit->text();
    QString password = ui->passwordEdit->text();

    if (dbManager->connectToDatabase(dbType, host, dbName, user, password)) {
        QMessageBox::information(this, "Success", "Connected to database!");
    } else {
        QMessageBox::critical(this, "Error", dbManager->getLastError());
    }
}

void MainWindow::on_executeQueryButton_clicked() {
    QString queryText = ui->queryEdit->toPlainText();
    dbManager->getModel()->setQuery(queryText);

    if (dbManager->getModel()->lastError().isValid()) {
        QMessageBox::critical(this, "Query Error", dbManager->getModel()->lastError().text());
    }
}

void MainWindow::on_exportButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "Export CSV", "", "*.csv");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream stream(&file);
    // Запись заголовков таблицы
    for (int col = 0; col < dbManager->getModel()->columnCount(); ++col)
        stream << dbManager->getModel()->headerData(col, Qt::Horizontal).toString() << ",";

    stream << "\n";

    // Запись данных таблицы
    for (int row = 0; row < dbManager->getModel()->rowCount(); ++row) {
        for (int col = 0; col < dbManager->getModel()->columnCount(); ++col)
            stream << dbManager->getModel()->data(dbManager->getModel()->index(row, col)).toString() << ",";
        stream << "\n";
    }

    file.close();
    QMessageBox::information(this, "Exported", "Data exported successfully!");
}
