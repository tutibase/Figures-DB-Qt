#ifndef MYCOLLECTION_H
#define MYCOLLECTION_H
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsScene>
//#include <QtCore>
#include "figures.h"
#include "myline.h"

class myCollection {

public:
    myCollection(int squares = 0, int ellipses = 0, int triangles = 0);
    void newFigure(QGraphicsScene *scene, int type = -1, int ID = -1);
    std::vector<QAbstractGraphicsShapeItem*> items;
    int maxiZ;
    int max_id;
    std::vector<myLine*> lines;

    int squares;
    int ellipses;
    int triangles;
};

#endif // MYCOLLECTION_H
