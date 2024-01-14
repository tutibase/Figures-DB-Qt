#ifndef MYLINE_H
#define MYLINE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsLineItem >

class myLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    myLine(qreal x1, qreal y1, qreal x2, qreal y2, int id1, int id2);
    int getId1() {return id1;};
    int getId2() {return id2;};

private:
    int id1;
    int id2;

public slots:
    void moveP1(QPointF newPos);
    void moveP2(QPointF newPos);
};

#endif // MYLINE_H
