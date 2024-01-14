#include "figures.h"
#include "mycollection.h"

MyRectItem::MyRectItem(int a, int b, int c, int d, myCollection *col) : QGraphicsRectItem(a,b,c,d)
{
    this->col = col;
}

MyRectItem::MyRectItem(int a, int b, int c, int d, myCollection *col, int id) : QGraphicsRectItem(a,b,c,d)
{
    this->id = id;
    this->col = col;
}

void MyRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setBrush(QBrush(Qt::green, Qt::SolidPattern));
    up(&(col->maxiZ));
    QGraphicsRectItem::mousePressEvent(event);
}

void MyRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setBrush(QBrush(Qt::red, Qt::SolidPattern));
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void MyRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsRectItem::mouseMoveEvent(event);
}

void MyRectItem::up(int *maxiZ){
    if (*maxiZ == this->zValue()) return;
    setZValue(++(*maxiZ));
}

QVariant MyRectItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();

        int xOffset = rect().x() + rect().width()/2;
        int yOffset = rect().y() + rect().height()/2;

        newPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);
        emit movedItem(newPos);

    }
    return QGraphicsItem::itemChange(change, value);
}

//////////////////////////////////////////////////////////////////////////////////////
//Ellipse
MyEllipseItem::MyEllipseItem(int a, int b, int c, int d, myCollection *col) : QGraphicsEllipseItem(a,b,c,d)
{
    this->col = col;
}

MyEllipseItem::MyEllipseItem(int a, int b, int c, int d, myCollection *col, int id) : QGraphicsEllipseItem(a,b,c,d)
{
    this->id = id;
    this->col = col;
}

void MyEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setBrush(QBrush(Qt::green, Qt::SolidPattern));
    up(&(col->maxiZ));
    QGraphicsEllipseItem::mousePressEvent(event);
}

void MyEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setBrush(QBrush(Qt::red, Qt::SolidPattern));
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void MyEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void MyEllipseItem::up(int *maxiZ){
    if (*maxiZ == this->zValue()) return;
    setZValue(++(*maxiZ));
}


QVariant MyEllipseItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();

        int xOffset = rect().x() + rect().width()/2;
        int yOffset = rect().y() + rect().height()/2;

        newPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);
        emit movedItem(newPos);

    }
    return QGraphicsItem::itemChange(change, value);
}

//////////////////////////////////////////////////////////////////////////////////////
//Triangle

myTriangle::myTriangle(myCollection *col){
    QPolygonF polygonT;
    QPointF first(-40, 0);
    QPointF second(0, -60);
    QPointF third(40, 0);
    polygonT << first << second << third << first;
    setPolygon(polygonT);

    this->col = col;
}

myTriangle::myTriangle(myCollection *col, int id) : myTriangle(col){
    this->id = id;
}

void myTriangle::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setBrush(QBrush(Qt::green, Qt::SolidPattern));
    up(&(col->maxiZ));
    QGraphicsPolygonItem::mousePressEvent(event);
}

void myTriangle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setBrush(QBrush(Qt::red, Qt::SolidPattern));
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void myTriangle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void myTriangle::up(int *maxiZ){
    if (*maxiZ == this->zValue()) return;
    setZValue(++(*maxiZ));
}

QVariant myTriangle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();

        QRectF rect = this->boundingRect();
        int xOffset = rect.x() + 30;
        int yOffset = rect.y() + 30;

        newPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);
        emit movedItem(newPos);

    }
    return QGraphicsItem::itemChange(change, value);
}








