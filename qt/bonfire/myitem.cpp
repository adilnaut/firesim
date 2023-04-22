#include "myitem.h"
#include "mainwindow.h"




MyItem::MyItem( int x, int y, int type)
{
  this->x = x;
  this->y = y;
  this->type = type;
  this->humidity = 0;


}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QBrush Brush(Qt::red);

    if (this->type == 0){
      Brush.setColor(Qt::green);
    }
    else{
      if (this->type == 1){
        Brush.setColor(Qt::gray);
      } else{
        if (this->type == 2){
            Brush.setColor(Qt::red);
        } else{
            Brush.setColor(Qt::blue);
        }
      }

    }
    QRectF rec = boundingRect();
    painter->fillRect(rec, Brush );
    painter->drawRect(rec);
    widget->update();

}
QRectF MyItem::boundingRect() const
{
  int width = 9;
  return QRect(width*this->x, width* this->y,width, width);
}
void MyItem::advance( int phase)
{
    if (phase == 0){
        return;
    }
    if (this->type == 0){

      int num_neighbours = this->neighbours.size();

      for(int i = 0; i < num_neighbours; i ++){
          MyItem *neighbour = this->neighbours[i];
          if (neighbour->type == 2){
              int rand_num = rand()%100;
              if (rand_num < 59){
                rand_num += this->humidity * 20;
              }
              if (rand_num > 65){
                  break;
              }
              this->type = 2;
              this->fire_timeout = neighbour->fire_timeout - 1 - this->humidity;
              break;
          }
        }
    } else{
      if (this->type == 2){
        if (this->fire_timeout <= 0){
          this->type = 1;
          return;
        }
        this->fire_timeout = this->fire_timeout -  1;
      }
    }
    this->setPos(mapToParent(0,0));
}

void MyItem::mousePressEvent( QGraphicsSceneMouseEvent *event)
{
    int timeout = 10;
    this->type = 2;
    this->fire_timeout = timeout;
    int num_neighbours = this->neighbours.size();

    for(int i = 0; i < num_neighbours; i ++){
        if (i == 0 || i == 2 || i == 5 || i == 7){
            continue;
        }
        MyItem *neighbour = this->neighbours[i];
        neighbour->type = 2;
        this->fire_timeout = timeout;

      }
    this->setPos(mapToParent(0,0));
}

void MyItem::addNeighbour(MyItem* item)
{
    this->neighbours << item;
}

QList<MyItem *> MyItem::getNeighbours()
{
    return this->neighbours;
}
