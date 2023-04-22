#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Rectangle :public QGraphicsRectItem
{
public:
    Rectangle(int x, int y, int type );
};

#endif // RECTANGLE_H
