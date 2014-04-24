#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "glwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMenuBar *menuBar;

    QToolBar *toolbar;
    QStatusBar *statusbar;
    
    QMenu *fileMenu;
    QAction *exitAction;

    QMenu *shadingMenu;
    QAction *wireframeAction;
    QAction *flatAction;
    QAction *gouraudAction;
    QAction *phongAction;
    QActionGroup *shadingGroup;

    QAction *aboutAction;

    GLWidget *glwidget;
public slots:
    void showAboutBox();
};

#endif // MAINWINDOW_H
