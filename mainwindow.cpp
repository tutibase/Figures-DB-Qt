#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, ui->graphicsView->width()-20, ui->graphicsView->height()-20);

    ui->graphicsView->setScene(scene);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./testDB.db");
    if (db.open()) qDebug("opened");
    else qDebug("not opened");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Figures(ID INT, type TEXT, connected_to TEXT, count INT);");

    model = new QSqlTableModel(this, db);
    model->setTable("Figures");
    model->select();

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->textEdit->setText("ID 1");
    ui->textEdit_2->setText("ID 2");

    QSqlRecord record;
    model->setFilter("type = 'Square'");
    record = model->record(0);
    int squares = record.value("count").toInt();

    model->setFilter("type = 'Ellipse'");
    record = model->record(0);
    int ellipses = record.value("count").toInt();

    model->setFilter("type = 'Triangle'");
    record = model->record(0);
    int triangles = record.value("count").toInt();
    collection = new myCollection(squares, ellipses, triangles);

    model->setFilter("");

    drawFromDB();

    ui->tableView->setItemDelegateForColumn(3, new myDelegate);
    ui->tableView->setColumnWidth(3, 120);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_toolButton_clicked() { // new
    on_showAllFigBtn_clicked();
    collection->newFigure(scene);

    QString type;
    int count;

    QSqlRecord record = model->record();
    if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(collection->items[collection->items.size()-1])){
        type = QString("Ellipse");
        record.setValue("count", ++collection->ellipses);
        count = collection->ellipses;
    }

    if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(collection->items[collection->items.size()-1])){
        type = QString("Triangle");
        record.setValue("count", ++collection->triangles);
        count = collection->triangles;
    }

    if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(collection->items[collection->items.size()-1])){
        type = QString("Square");
        record.setValue("count", ++collection->squares);
        count = collection->squares;
    }

    model->setFilter(QString("type = '%1'").arg(type));

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord rec = model->record(i);
        rec.setValue("count", count);
        model->setRecord(i, rec);
    }
    model->submitAll();

    model->setFilter("");

    record.setValue("ID", collection->max_id);
    record.setValue("type", type);
    model->insertRecord(model->rowCount(), record);
    model->submitAll();
}

void MainWindow::on_toolButton_2_clicked() { // delete
    on_showAllFigBtn_clicked();
    if (collection->items.size() == 0) return;

    bool flag = 0;
    QAbstractGraphicsShapeItem* deleter;
    for (int i=0; i < collection->items.size()-1; i++){
        if (collection->items[i]->isSelected()){
            flag = 1;
            deleter = collection->items[i];
        }

        if (flag) collection->items[i] = collection->items[i+1];
    }

    if (!flag) deleter = collection->items[collection->items.size()-1];

    int deleterID = 0;
    int delVal;
    if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(deleter)){
        deleterID = tmp->id;
        delVal = --collection->ellipses;
        model->setFilter("type = 'Ellipse'");
    }

    if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(deleter)){
        deleterID = tmp->id;
        delVal = --collection->triangles;
        model->setFilter("type = 'Triangle'");
    }

    if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(deleter)){
        deleterID = tmp->id;
        delVal = --collection->squares;
        model->setFilter("type = 'Square'");
    }

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord record = model->record(i);
        record.setValue("count", delVal);
        model->setRecord(i, record);
    }
    model->submitAll();

    model->setFilter(QString("id = %1").arg(deleterID));
    model->select();
    model->removeRows(0, 1);
    model->submitAll();
    model->setFilter("");

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord record = model->record(i);
        QStringList connectIDS = record.value("connected_to").toString().split(u',');
        QString tmp = "";
        for (int j = 0; j < connectIDS.size(); j++){
            if (connectIDS[j].toInt() == deleterID) continue;
            if (tmp != "")
                tmp.append(QString(", %1").arg(connectIDS[j].toInt()));
            else
                tmp.append(connectIDS[j]);
        }
        record.setValue("connected_to", tmp);
        model->setRecord(i, record);
    }
    model->submitAll();

    for (int i=0; i<collection->lines.size(); i++) {
        if (collection->lines[i]->getId1() == deleterID or collection->lines[i]->getId2() == deleterID){
            scene->removeItem(collection->lines[i]);
            delete collection->lines[i];
            for (int j = i; j<collection->lines.size()-1; j++) {
                collection->lines[j] = collection->lines[j+1];
            }
            collection->lines.pop_back();
            i--;
        }
    }

    scene->removeItem(deleter);
    delete deleter;
    collection->items.pop_back();
}


void MainWindow::on_connectorButton_clicked()
{
    bool flagId1 = 0;
    bool flagId2 = 0;

    int id1 = ui->textEdit->toPlainText().toInt();
    int id2 = ui->textEdit_2->toPlainText().toInt();

    if (id2 == id1) return;
    if (id2 < id1){
        int tmp = id1;
        id1 = id2;
        id2 = tmp;
    }

    for (int i=0; i<collection->lines.size();i++){
        if (collection->lines[i]->getId1() == id1 and collection->lines[i]->getId2() == id2) return;
    }

    myLine *line = new myLine(0, 0, 0, 0, id1, id2);
    for (int i=0; i < collection->items.size(); i++){
        if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(collection->items[i])) {
            QPointF newPos = QPointF(tmp->x() + tmp->rect().width()/2 + tmp->rect().x(),
                                     tmp->y() + tmp->rect().height()/2 + tmp->rect().y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

        if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(collection->items[i])) {
            QPointF newPos = QPointF(tmp->x() + tmp->rect().width()/2 + tmp->rect().x(),
                                     tmp->y() + tmp->rect().height()/2 + tmp->rect().y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

        if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(collection->items[i])) {
            QRectF rect = tmp->boundingRect();
            QPointF newPos = QPointF(tmp->x() + 30 + rect.x(),
                                     tmp->y() + 30 + rect.y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

    }

    line->setZValue(-1);
    scene->addItem(line);
    if (!flagId1 or !flagId2) {
        scene->removeItem(line);
        delete line;
        return;
    }

    model->setFilter(QString("id = %1").arg(id1));
    QSqlRecord record = model->record(0);
    if (record.value("connected_to").toString() != "")
        record.setValue("connected_to", (record.value("connected_to").toString()).append(QString(", %1").arg(id2)));
    else
        record.setValue("connected_to", (record.value("connected_to").toString()).append(QString("%1").arg(id2)));

    model->setRecord(0, record);
    model->submitAll();

    model->setFilter(QString("id = %1").arg(id2));
    record = model->record(0);
    if (record.value("connected_to").toString() != "")
        record.setValue("connected_to", (record.value("connected_to").toString()).append(QString(", %1").arg(id1)));
    else
        record.setValue("connected_to", (record.value("connected_to").toString()).append(QString("%1").arg(id1)));
    model->setRecord(0, record);
    model->submitAll();
    model->setFilter("");

    collection->lines.push_back(line);
}

///////////////////////////////////////////////////////////////////////////////
/// show and hide buttons
///
void MainWindow::hideByID(int hideID) {
    for (int i=0; i < collection->items.size(); i++){
        QAbstractGraphicsShapeItem * hider = collection->items[i];
        if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(hider)){
            if (hideID == tmp->id) tmp->hide();
        }

        if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(hider)){
            if (hideID == tmp->id) tmp->hide();
        }

        if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(hider)){
            if (hideID == tmp->id) tmp->hide();
        }
    }
    for (int i=0; i < collection->lines.size(); i++){
        if (collection->lines[i]->getId1() == hideID or collection->lines[i]->getId2() == hideID)
            collection->lines[i]->hide();
    }
}

void MainWindow::showByID(int showID) {
    for (int i=0; i < collection->items.size(); i++){
        QAbstractGraphicsShapeItem * hider = collection->items[i];
        if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(hider)){
            if (showID == tmp->id) tmp->show();
        }

        if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(hider)){
            if (showID == tmp->id) tmp->show();
        }

        if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(hider)){
            if (showID == tmp->id) tmp->show();
        }
    }

    for (int i=0; i < collection->lines.size(); i++){
        if (collection->lines[i]->getId1() == showID or collection->lines[i]->getId2() == showID)
            collection->lines[i]->show();
    }
}


void MainWindow::drawFromDB(){
    QSqlRecord figure;
    for (int i = 0; i < model->rowCount(); i++){
        figure = model->record(i);
        int figID = figure.value("ID").toInt();
        if (figID > collection->max_id)
            collection->max_id = figID;

        if (figure.value("type").toString() == "Triangle")
            collection->newFigure(scene, 2, figID);
        if (figure.value("type").toString() == "Square")
            collection->newFigure(scene, 0, figID);
        if (figure.value("type").toString() == "Ellipse")
            collection->newFigure(scene, 1, figID);

        QStringList connectIDS = figure.value("connected_to").toString().split(u',');
        for (int j = 0; j < connectIDS.size(); j++){
            connectFromDB(figID, connectIDS[j].toInt());
        }

    }
}

void MainWindow::connectFromDB(int id1, int id2) {
    bool flagId1 = 0;
    bool flagId2 = 0;

    if (id2 == id1) return;
    if (id2 < id1){
        int tmp = id1;
        id1 = id2;
        id2 = tmp;
    }

    for (int i=0; i<collection->lines.size();i++){
        if (collection->lines[i]->getId1() == id1 and collection->lines[i]->getId2() == id2) return;
    }

    myLine *line = new myLine(0, 0, 0, 0, id1, id2);
    for (int i=0; i < collection->items.size(); i++){
        if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(collection->items[i])) {
            QPointF newPos = QPointF(tmp->x() + tmp->rect().width()/2 + tmp->rect().x(),
                                     tmp->y() + tmp->rect().height()/2 + tmp->rect().y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

        if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(collection->items[i])) {
            QPointF newPos = QPointF(tmp->x() + tmp->rect().width()/2 + tmp->rect().x(),
                                     tmp->y() + tmp->rect().height()/2 + tmp->rect().y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

        if (myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(collection->items[i])) {
            QRectF rect = tmp->boundingRect();
            QPointF newPos = QPointF(tmp->x() + 30 + rect.x(),
                                     tmp->y() + 30 + rect.y());
            if (tmp->id == id1) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP1(QPointF)));
                line->setLine(QLineF(newPos, line->line().p2()));
                flagId1 = 1;
            }
            if (tmp->id == id2) {
                connect(tmp, SIGNAL(movedItem(QPointF)), line, SLOT(moveP2(QPointF)));
                line->setLine(QLineF(line->line().p1(), newPos));
                flagId2 = 1;
            }
        }

    }

    line->setZValue(-1);
    scene->addItem(line);
    if (!flagId1 or !flagId2) {
        scene->removeItem(line);
        delete line;
        return;
    }

    collection->lines.push_back(line);
}

void MainWindow::on_hideSquaresBtn_clicked()
{
    QString filter;
    if (model->filter() != "")
        filter = QString("type = 'Square' and %1").arg(model->filter());
    else
        filter = QString("type = 'Square'");
    model->setFilter(filter);

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord record = model->record(i);
        hideByID(record.value("ID").toInt());
    }
    model->setFilter(QString("NOT ").append(filter));
}


void MainWindow::on_hideTrianglesBtn_clicked()
{
    QString filter;
    if (model->filter() != "")
        filter = QString("type = 'Triangle' and %1").arg(model->filter());
    else
        filter = QString("type = 'Triangle'");
    model->setFilter(filter);

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord record = model->record(i);
        hideByID(record.value("ID").toInt());
    }
    model->setFilter(QString("NOT ").append(filter));
}


void MainWindow::on_hideEllipsesBtn_clicked()
{
    QString filter;
    if (model->filter() != "")
        filter = QString("type = 'Ellipse' and %1").arg(model->filter());
    else
        filter = QString("type = 'Ellipse'");
    model->setFilter(filter);

    for (int i = 0; i < model->rowCount(); i++) {
        QSqlRecord record = model->record(i);
        hideByID(record.value("ID").toInt());
    }
    model->setFilter(QString("NOT ").append(filter));
}


// hide figure whose ID is in the ID1 field
void MainWindow::on_hideID1Btn_clicked() {
    int hideID = ui->textEdit->toPlainText().toInt();

    QString filter;
    if (model->filter() != "")
        filter = QString("id = %1 and %2").arg(hideID).arg(model->filter());
    else
        filter = QString("id = %1").arg(hideID);
    model->setFilter(filter);

    model->setFilter(filter);
    QSqlRecord record = model->record(0);
    hideByID(record.value("ID").toInt());
    model->setFilter(QString("NOT ").append(filter));
}


void MainWindow::on_showAllFigBtn_clicked()
{
    model->setFilter("");
    for (int i = 0; i < model->rowCount(); i++){
        QSqlRecord record = model->record(i);
        showByID(record.value("ID").toInt());
    }
}

void MainWindow::graphicsDelete(){
    if (collection->items.size() == 0) return;

    for (int i = 0; i < model->rowCount(); i++) {
        bool flag = 0;
        QAbstractGraphicsShapeItem* deleter;
        for (int i=0; i < collection->items.size()-1; i++){
            if (collection->items[i]->isSelected()){
                flag = 1;
                deleter = collection->items[i];
            }

            if (flag) collection->items[i] = collection->items[i+1];
        }

        if (!flag) deleter = collection->items[collection->items.size()-1];

        int deleterID = 0;
        if(MyEllipseItem *tmp = qgraphicsitem_cast<MyEllipseItem*>(deleter))
            deleterID = tmp->id;

        if(myTriangle *tmp = qgraphicsitem_cast<myTriangle*>(deleter))
            deleterID = tmp->id;

        if(MyRectItem *tmp = qgraphicsitem_cast<MyRectItem*>(deleter))
            deleterID = tmp->id;

        for (int i=0; i<collection->lines.size(); i++) {
            if (collection->lines[i]->getId1() == deleterID or collection->lines[i]->getId2() == deleterID){
                scene->removeItem(collection->lines[i]);
                delete collection->lines[i];
                for (int j = i; j<collection->lines.size()-1; j++) {
                    collection->lines[j] = collection->lines[j+1];
                }
                collection->lines.pop_back();
                i--;
            }
        }

        scene->removeItem(deleter);
        delete deleter;
        collection->items.pop_back();
    }
}
