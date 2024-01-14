#ifndef FIGURES_H
#define FIGURES_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>


class myCollection;

class MyRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    myCollection *col;

public:
    MyRectItem(int a, int b, int c, int d, myCollection *col);
    MyRectItem(int a, int b, int c, int d, myCollection *col, int id);
    int id;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void movedItem(QPointF);

public slots:
    void up(int *maxiZ);
};

////////////////////////////////////////////////////////////////////////////////////////
class MyEllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
private:
    myCollection *col;

public:
    MyEllipseItem(int a, int b, int c, int d, myCollection *col);
    MyEllipseItem(int a, int b, int c, int d, myCollection *col, int id);
    int id;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void movedItem(QPointF);

public slots:
    void up(int *maxiZ);
};


////////////////////////////////////////////////////////////////////////////////////////
class myTriangle : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT
private:
    myCollection *col;

public:
    myTriangle(myCollection *col);
    myTriangle(myCollection *col, int id);
    int id;

protected:


    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void movedItem(QPointF);

public slots:
    void up(int *maxiZ);
};


#endif // FIGURES_H
