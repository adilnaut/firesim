#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myitem.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene = new QGraphicsScene(this);

//    QOpenGLWidget *glWidget = new QOpenGLWidget();

//    ui->drawScreen->setViewport(glWidget);
//    ui>graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    ui->graphicsView->setViewport(glWidget);
//    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setScene(scene);
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene->setSceneRect(0, 0, 0, 0);

    n = 70;


    // int **terrain = new int*[n];
    // for (int i = 0; i < n; ++i) {
    //   terrain[i] = new int[n];
    // }
    //
    // for (int i = 0; i < n; ++i) {   // for each row
    //   for (int j = 0; j < n; ++j) { // for each column
    //     if (rand()%9 == 0){
    //       terrain[i][j] = 0;
    //     } else{
    //       terrain[i][j] = 1;
    //     }
    //   }
    // }

//    QMap<QPair<int, int>, MyItem*> item_map;
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          int terrain = 1;
          if (rand()%9 == 0){
            terrain = 0;
          }
          MyItem *item = new MyItem( i, j, terrain);
          scene->addItem(item);
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          item_map[pair] = item;
      }
    }

    int neighbours[8][2] = {
      {-1, -1} , {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };


    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];

          for(int k = 0; k < 8; k++){

            QPair<int, int> t_pair;

            t_pair.first = i + neighbours[k][0];
            t_pair.second = j + neighbours[k][1];

            if (t_pair.first < 0 || t_pair.first >= n-1){
              break;
            }
            if (t_pair.second < 0 || t_pair.second >= n-1){
              break;
            }
            item->addNeighbour(item_map[t_pair]);
          }
      }
    }

    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          int num_neighbours = item->neighbours.size();
          if (item->type == 0 ){
            for(int k = 0; k < num_neighbours; k++){
                MyItem *neighbour = item->neighbours[k];
                if (neighbour->type == 1 && rand()%4 == 0){
                  neighbour->type = 0;
                }
            }
          }
      }
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_play_clicked()
{
    timer->start(1000);
}

void MainWindow::on_pause_clicked()
{
    timer->stop();
}

void MainWindow::on_stop_clicked()
{
    timer->stop();
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          int terrain = 1;
          if (rand()%9 == 0){
            terrain = 0;
          }
          item->type = terrain;
      }
    }
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          int num_neighbours = item->neighbours.size();
          if (item->type == 0 ){
            for(int k = 0; k < num_neighbours; k++){
                MyItem *neighbour = item->neighbours[k];
                if (neighbour->type == 1 && rand()%4 == 0){
                  neighbour->type = 0;
                }
            }
          } else{
            item->type = 1;
          }
      }
    }

}

void MainWindow::on_play_5_clicked()
{
//    timer->stop();
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          item->advance(1);
      }
    }
}

//generate trees
void MainWindow::on_pushButton_clicked()
{
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          int num_neighbours = item->neighbours.size();
          if (item->type == 0 ){
            for(int k = 0; k < num_neighbours; k++){
                MyItem *neighbour = item->neighbours[k];
                if (neighbour->type == 1 && rand()%9 == 0){
                  neighbour->type = 0;
                }
            }
          } else{
            item->type = 1;
          }
      }
    }
}

//add water
void MainWindow::on_pushButton_2_clicked()
{
    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          if (rand()%250 == 0){
             item->type = 3;
          }
      }
    }

    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
          QPair<int, int> pair;
          pair.first = i;
          pair.second = j;
          MyItem *item = item_map[pair];
          int num_neighbours = item->neighbours.size();
          if (item->type == 3 ){
            for(int k = 0; k < num_neighbours; k++){
                MyItem *neighbour = item->neighbours[k];
                if ( (neighbour->type == 1 || neighbour->type == 0) && rand()%4 == 0){
                  neighbour->type = 3;
                }
            }
          }
      }
    }

    for (int i = 0; i < n; ++i) {   // for each row
      for (int j = 0; j < n; ++j) { // for each column
        QPair<int, int> pair;
        pair.first = i;
        pair.second = j;
        MyItem *item = item_map[pair];
        int num_neighbours = item->neighbours.size();
        for(int k = 0; k < num_neighbours; k++){
          if (item->type == 3 ){
              MyItem *neighbour = item->neighbours[k];
              if (neighbour->type == 0){
                neighbour->humidity = 2;
                int grand_num_neighbours = neighbour->neighbours.size();
                for(int l = 0; l < grand_num_neighbours; l++){
                  MyItem *grand_neighbour = neighbour->neighbours[l];
                    if (grand_neighbour->type == 0){
                        grand_neighbour->humidity = 1;
                    }
                }
              }
          }
        }
      }
    }

}
