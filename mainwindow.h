#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "figures/mycollection.h"
#include "figures/figures.h"
#include "figures/myline.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "mydelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_connectorButton_clicked();

    void hideByID(int hideID);

    void showByID(int showID);

    void drawFromDB();
    void connectFromDB(int id1, int id2);
    void graphicsDelete();

    void on_hideSquaresBtn_clicked();

    void on_hideTrianglesBtn_clicked();

    void on_hideEllipsesBtn_clicked();

    void on_hideID1Btn_clicked();

    void on_showAllFigBtn_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    myCollection *collection;

    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
};
#endif // MAINWINDOW_H
