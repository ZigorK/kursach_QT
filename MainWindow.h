#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "DatabaseManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_executeQueryButton_clicked();
    void on_exportButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager *dbManager;  // Используем наш класс для работы с БД
};

#endif // MAINWINDOW_H
