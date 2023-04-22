/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QPushButton *play;
    QPushButton *pause;
    QPushButton *stop;
    QPushButton *play_5;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(807, 808);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(30, 60, 751, 691));
        play = new QPushButton(centralwidget);
        play->setObjectName(QStringLiteral("play"));
        play->setGeometry(QRect(30, 10, 51, 41));
        play->setStyleSheet(QLatin1String("#play{\n"
"background-color:transparent;\n"
"border-image: url(:/images/play.png);\n"
"background:none;\n"
"border:none;\n"
"background-repeat:none;\n"
"}\n"
"#play:pressed\n"
"{\n"
"  border-image: url(:/images/play_pressed.png);\n"
"}"));
        play->setCheckable(false);
        play->setAutoDefault(false);
        play->setFlat(false);
        pause = new QPushButton(centralwidget);
        pause->setObjectName(QStringLiteral("pause"));
        pause->setGeometry(QRect(150, 10, 51, 41));
        pause->setStyleSheet(QLatin1String("#pause{\n"
"background-color:transparent;\n"
"border-image: url(:/images/pause.png);\n"
"background:none;\n"
"border:none;\n"
"background-repeat:none;\n"
"}\n"
"#pause:pressed\n"
"{\n"
"  border-image: url(:/images/pause_pressed.png);\n"
"}"));
        stop = new QPushButton(centralwidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(210, 10, 51, 41));
        stop->setStyleSheet(QLatin1String("#stop{\n"
"background-color:transparent;\n"
"border-image: url(:/images/stop.png);\n"
"background:none;\n"
"border:none;\n"
"background-repeat:none;\n"
"}\n"
"#stop:pressed\n"
"{\n"
"  border-image: url(:/images/stop_pressed.png);\n"
"}"));
        play_5 = new QPushButton(centralwidget);
        play_5->setObjectName(QStringLiteral("play_5"));
        play_5->setGeometry(QRect(90, 10, 51, 41));
        play_5->setStyleSheet(QLatin1String("#play{\n"
"background-color:transparent;\n"
"border-image: url(:/images/play.png);\n"
"background:none;\n"
"border:none;\n"
"background-repeat:none;\n"
"}\n"
"#play:pressed\n"
"{\n"
"  border-image: url(:/images/play_pressed.png);\n"
"}"));
        play_5->setCheckable(false);
        play_5->setAutoDefault(false);
        play_5->setFlat(false);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(520, 10, 121, 41));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(650, 10, 121, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 807, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        play->setText(QString());
        pause->setText(QString());
        stop->setText(QString());
        play_5->setText(QApplication::translate("MainWindow", ">>", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Generate Trees", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "Generate lakes", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
