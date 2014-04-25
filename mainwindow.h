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

    QSlider *tesselationSlider;

    QAction *aboutAction;

    GLWidget *glwidget;

    void createActions();
    void createMenus();
public slots:
    void showAboutBox();
};

#endif // MAINWINDOW_H
