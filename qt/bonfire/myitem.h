#ifndef MYITEM_H
#define MYITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
//#include <QOpenGLWidget>

class MyItem : public QGraphicsItem
{
public:
    MyItem(int x, int y, int type);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void advance(int phase);
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event);
    void addNeighbour(MyItem *item);
    QList<MyItem*> getNeighbours();
    int type;
    QList<MyItem*> neighbours;
    int humidity;
  private:
    int  x;
    int  y;
    int fire_timeout;

};

#endif // MYITEM_H
