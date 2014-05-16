#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "glwidget.h"
#include "glwidgetcontext.h"

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

    QSlider *tesselationSlider;

    QMenu *viewMenu;
    QToolButton *viewDropButton;
    QAction *singleViewAction;
    QAction *dualViewAction;
    QAction *quadViewAction;
    QActionGroup *viewportGroup;
    QAction *resetCameraAction;

    QMenu *helpMenu;
    QAction *aboutAction;

    GLWidgetContext *glWidgetContext;
    GLWidget *perspectiveGLWidget;
    GLWidget *topGLWidget;

    QSplitter *topSplitter;
    void createActions();
    void createMenus();
    void createToolbar();
public slots:
    void showAboutBox();
};

#endif // MAINWINDOW_H
