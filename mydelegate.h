#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QDebug>

class myDelegate : public QItemDelegate
{
public:
    myDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        int count = index.model()->data(index, Qt::DisplayRole).toInt();
        if (count < 4) count = 1;
        else if (count < 10) count = 2;
        else count = 3;


        QModelIndex typeIndex = index.model()->index(index.row(), 1);
        QString type = index.model()->data(typeIndex, Qt::DisplayRole).toString();

        painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));

        QRect rect = option.rect;
        rect.setLeft(rect.x());
        rect.setWidth(rect.width() - 10);
        rect.translate(-44, 0);

        for (int i = 0; i < count; i++) {

            if (type == "Ellipse") {
                painter->drawEllipse(rect.center(), 7, 7);
            }

            if (type == "Triangle"){
                QPolygonF polygonT;
                QPointF first(rect.center().x()-7, rect.center().y()+7);
                QPointF second(rect.center().x(), rect.center().y()-7);
                QPointF third(rect.center().x()+7, rect.center().y()+7);
                polygonT << first << second << third << first;

                painter->drawPolygon(polygonT);
            }

            if (type == "Square") {
                painter->drawRect(rect.center().x()-7, rect.center().y()-7, 14, 14);
            }
            rect.translate(17, 0);
        }
    }
};

#endif // MYDELEGATE_H
