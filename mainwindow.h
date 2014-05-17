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

    Scene *scene;
    SceneGraph *currentNode;
    GLWidgetContext *glWidgetContext;
    GLWidget *perspectiveGLWidget;
    GLWidget *frontGLWidget;
    GLWidget *topGLWidget;
    GLWidget *rightGLWidget;

    QTreeView *sceneOutliner;
    QDockWidget *outlinerDock;
    
    QSplitter *topSplitter;
    QSplitter *bottomSplitter;
    QSplitter *sideSplitter;
    
    void createActions();
    void createMenus();
    void createToolbar();
    void setupGL();
public slots:
    void translateNode(double x, double y, double z);
    void rotateNode(QQuaternion *q);
    void showAboutBox();
    void setSingleView();
    void setDualView();
    void setQuadView();
    void changeCurrentNode(const QModelIndex &current, const QModelIndex &previous);

signals:
    void updateGL();
};

#endif // MAINWINDOW_H
