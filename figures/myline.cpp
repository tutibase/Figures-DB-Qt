#include "myline.h"

myLine::myLine(qreal x1, qreal y1, qreal x2, qreal y2, int id1, int id2) : QGraphicsLineItem(x1, y1, x2, y2)
{
    this->id1 = id1;
    this->id2 = id2;
}


void myLine::moveP1(QPointF newPos){
    this->setLine(QLineF(newPos, this->line().p2()));
}

void myLine::moveP2(QPointF newPos){
    this->setLine(QLineF(this->line().p1(), newPos));
}
