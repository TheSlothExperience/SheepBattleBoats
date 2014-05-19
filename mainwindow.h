#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "glwidget.h"
#include "glwidgetcontext.h"
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

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

    QAction *cameraModeAction;
    QAction *objectModeAction;
    QActionGroup *interactionGroup;
    QAction *resetCameraAction;

    QAction *addCubeAction;
    QAction *addTorusAction;
    QAction *addCylinderAction;
    QAction *addConeAction;
    QAction *addSphereAction;

    QMenu *helpMenu;
    QAction *aboutAction;

    Scene *scene;
    SceneGraph *currentNode;
    GLWidgetContext *glWidgetContext;
    GLWidget *perspectiveGLWidget;
    GLWidget *frontGLWidget;
    GLWidget *topGLWidget;
    GLWidget *rightGLWidget;
    std::vector<GLWidget*> widgetList;

    QTreeView *sceneOutliner;
    QDockWidget *outlinerDock;
    
    QSplitter *topSplitter;
    QSplitter *bottomSplitter;
    QSplitter *sideSplitter;

    template <class UnaryFunction >
    void mapWidgets(UnaryFunction f);
    
    void createActions();
    void createMenus();
    void createToolbar();
    void setupGL();
public slots:
    void addCube();
    void addCone();
    void addCylinder();
    void addSphere();
    void addTorus();
    
    void translateNode(double x, double y, double z);
    void rotateNode(QQuaternion *q);

    void setObjectInteraction();
    void setCameraInteraction();
    void setActiveViewport(GLWidget *active);
    
    void showAboutBox();
    void setSingleView();
    void setDualView();
    void setQuadView();
    void changeCurrentNode(const QModelIndex &current, const QModelIndex &previous);

signals:
    void updateGL();
};

#endif // MAINWINDOW_H
