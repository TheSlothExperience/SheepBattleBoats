#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <vector>
#include "glwidget.h"
#include "glwidgetcontext.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	QMenuBar *menuBar;

	QToolBar *toolbar;
	QStatusBar *statusbar;

	QMenu *fileMenu;
	QAction *exitAction;


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
    QAction *add3DModelAction;
    QAction *addLvlObjAction;

	QAction *addGroupAction;

	QAction *addLightAction;

    QAction *shootAction;

	QMenu *helpMenu;
	QAction *aboutAction;

	Scene *scene;
	SceneGraph *currentNode;
	GLWidgetContext *glWidgetContext;
	GLWidget *perspectiveGLWidget;
	GLWidget *frontGLWidget;
	GLWidget *topGLWidget;
	GLWidget *rightGLWidget;
	GLWidget *activeViewport;
	std::vector<GLWidget*> widgetList;

	QTreeView *sceneOutliner;
	QDockWidget *outlinerDock;

	QDockWidget *colorDock;
	QCheckBox *satShadowCheckBox;
    QCheckBox *set8bitCheckBox;
    QCheckBox *setScopeCheckBox;
    QCheckBox *setCrossHatchCheckBox;
	QSlider *redSlider;
	QSlider *blueSlider;
	QSlider *greenSlider;

	QSplitter *topSplitter;
	QSplitter *bottomSplitter;
	QSplitter *sideSplitter;

	template <class UnaryFunction >
	void mapWidgets(UnaryFunction f);

	void createActions();
	void createMenus();
	void createToolbar();
	void createColorDock();
	void setupGL();
    void load3DModel();

    void initGameLogic();
    void initLevel();
    void doMovements();

    bool wPressed=false;
    bool sPressed=false;
    bool dPressed=false;
    bool aPressed=false;

    float shootingHeight;
    bool shooting=false;

public slots:
	void addCube();
	void addCone();
	void addCylinder();
	void addSphere();
	void addTorus();
    void add3DModel();
    void addLvlObj();

	void addGroup();

	void addLight();
	void setSATShadows(bool set);
    void set8bit(bool set);
    void setScope(bool set);
    void setCrossHatch(bool set);

	void translateNode(double x, double y, double z);
	void rotateNode(QQuaternion *q);

	void setObjectInteraction();
	void setCameraInteraction();
	void setActiveViewport(GLWidget *active);

	void changeCurrent(QModelIndex q);
	void changeActiveId(int id);

	void changedColor();

	void showAboutBox();
	void setSingleView();
	void setDualView();
	void setQuadView();
	void changeCurrentNode(const QModelIndex &current, const QModelIndex &previous);
    void gameTick();
    void testCollisions();
    void shoot(float shootingHeight);

signals:
	void updateGL();
};

#endif // MAINWINDOW_H
