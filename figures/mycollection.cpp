#include "mycollection.h"

myCollection::myCollection(int squares, int ellipses, int triangles) {
    items.resize(0);
    lines.resize(0);
    maxiZ = 0;
    max_id = 0;

    this->squares = squares;
    this->ellipses = ellipses;
    this->triangles = triangles;
}

int randomBetween(int low, int high) {
    return (qrand() % ((high + 1) - low) + low);
}

void myCollection::newFigure(QGraphicsScene *scene, int type, int ID) {
    maxiZ++;

    if (type == -1) {
        type = rand() % 3;
        ID = ++max_id;
    }

    QAbstractGraphicsShapeItem *tmp;
    switch (type) {
    case 0:
    tmp = new MyRectItem(-30,-30,60,60, this, ID);
        break;
    case 1:
    tmp = new MyEllipseItem(-30,-30,60,60, this, ID);
        break;

    case 2:
    tmp = new myTriangle(this, ID);
        break;
    }

    tmp->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    tmp->setFlag(QGraphicsItem::ItemIsMovable, true);
    tmp->setFlag(QGraphicsItem::ItemIsSelectable, true);
    tmp->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    tmp->setPos(randomBetween(30, 1000), randomBetween(30, 500));
    scene->addItem(tmp);
    items.push_back(tmp);
}

