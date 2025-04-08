#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDriver>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), model(new QSqlQueryModel(this)) {
    ui->setupUi(this);
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    QString dbType = ui->dbTypeCombo->currentText(); // e.g., QMYSQL, QPSQL
    db = QSqlDatabase::addDatabase(dbType);
    db.setHostName(ui->hostEdit->text());
    db.setDatabaseName(ui->dbNameEdit->text());
    db.setUserName(ui->userEdit->text());
    db.setPassword(ui->passwordEdit->text());

    if (db.open()) {
        QMessageBox::information(this, "Success", "Connected to database!");
    } else {
        QMessageBox::critical(this, "Error", db.lastError().text());
    }
}

void MainWindow::on_executeQueryButton_clicked() {
    QString queryText = ui->queryEdit->toPlainText();
    model->setQuery(queryText, db);
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Query Error", model->lastError().text());
    }
}

void MainWindow::on_exportButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "Export CSV", "", "*.csv");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream stream(&file);
    for (int col = 0; col < model->columnCount(); ++col)
        stream << model->headerData(col, Qt::Horizontal).toString() << ",";

    stream << "\n";

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col)
            stream << model->data(model->index(row, col)).toString() << ",";
        stream << "\n";
    }

    file.close();
    QMessageBox::information(this, "Exported", "Data exported successfully!");
}

