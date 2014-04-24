#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMenuBar *menuBar;

    QMenu *fileMenu;
    QAction *exitAction;

    QMenu *shadingMenu;
    QAction *wireframeAction;
    QAction *flatAction;
    QAction *gouraudAction;
    QAction *phongAction;
    QActionGroup *shadingGroup;
};

#endif // MAINWINDOW_H
