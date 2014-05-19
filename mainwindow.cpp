#include "mainwindow.h"
#include "glwidget.h"
#include <qapplication.h>
#include "scene.h"
#include "camera.h"
#include "perspectivecamera.h"
#include "orthocamera.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello Cube!!");
    setMinimumSize(1000,800);
    
    setupGL();

    createActions();
    createMenus();
    createToolbar();

    sceneOutliner = new QTreeView();
    sceneOutliner->setWindowTitle(QObject::tr("Outliner"));
    sceneOutliner->setModel(scene);
    connect(sceneOutliner->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(changeCurrentNode(const QModelIndex&, const QModelIndex&)));
    
    outlinerDock = new QDockWidget(tr("Scene Outliner"), this);
    outlinerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outlinerDock->setWidget(sceneOutliner);
    addDockWidget(Qt::LeftDockWidgetArea, outlinerDock);
    
    //Status bar
    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);
    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

}

template <class UnaryFunction >
void MainWindow::mapWidgets(UnaryFunction f) {
    std::for_each(widgetList.begin(), widgetList.end(), f);
}

/*
 * Despair all ye who enter here, --
 * in this unholy mess of signals and slots.
 */
void MainWindow::setupGL() {
    //Set central OpenGL widget
    glWidgetContext = new GLWidgetContext();
    glWidgetContext->makeCurrent();
    glWidgetContext->initializeGL();
    
    //Set the scene and add a cube
    scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation());
    scene->setLightLocation(glWidgetContext->getLightPositionLocation());
    currentNode = scene->root();
    addCube();

    //Create the widgets
    perspectiveGLWidget = new GLWidget(this, glWidgetContext);
    widgetList.push_back(perspectiveGLWidget);
    perspectiveGLWidget->setPerspectiveCamera(1, 1, 3);
    
    frontGLWidget = new GLWidget(this, glWidgetContext);
    widgetList.push_back(frontGLWidget);
    frontGLWidget->setOrthoCamera(0, 0, 3);
    
    topGLWidget = new GLWidget(this, glWidgetContext);
    widgetList.push_back(topGLWidget);
    topGLWidget->setOrthoCamera(0, 3, 0);
    
    rightGLWidget = new GLWidget(this, glWidgetContext);
    widgetList.push_back(rightGLWidget);
    rightGLWidget->setOrthoCamera(3, 0, 0);


    //Map over the widgets setting the scene and connecting the signals
    mapWidgets([=](GLWidget *w){w->setScene(scene);});
    mapWidgets([=](GLWidget *w){w->setShaderProgram(glWidgetContext->getShaderProgram());});
    mapWidgets([=](GLWidget *w){w->setTextureProgram(glWidgetContext->getTextureProgram());});
    mapWidgets([=](GLWidget *w){w->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());});
    
    mapWidgets([=](GLWidget *w){
	    connect(w, SIGNAL(translate(double, double, double)), this, SLOT(translateNode(double, double, double)));
	});
    mapWidgets([=](GLWidget *w){
	    connect(w, SIGNAL(rotate(QQuaternion*)), this, SLOT(rotateNode(QQuaternion*)));
	});
    mapWidgets([=](GLWidget *w){
	    connect(this, SIGNAL(updateGL()), w, SLOT(forceGLupdate()));
	});
    mapWidgets([=](GLWidget *w){
	    connect(w, SIGNAL(switchActive(GLWidget*)), this, SLOT(setActiveViewport(GLWidget*)));
	});

    //Add the four viewports to the screen in splitters
    topSplitter = new QSplitter(this);
    topSplitter->addWidget(perspectiveGLWidget);
    topSplitter->addWidget(frontGLWidget);
    
    bottomSplitter = new QSplitter(this);
    bottomSplitter->addWidget(topGLWidget);
    bottomSplitter->addWidget(rightGLWidget);
    
    sideSplitter = new QSplitter(this);
    sideSplitter->setOrientation(Qt::Vertical);
    sideSplitter->addWidget(topSplitter);
    sideSplitter->addWidget(bottomSplitter);

    frontGLWidget->hide();
    bottomSplitter->hide();
    perspectiveGLWidget->setActive();
    
    setCentralWidget(sideSplitter);
}

void MainWindow::createToolbar() {
    //Toolbar
    toolbar = new QToolBar(this);

    tesselationSlider = new QSlider(Qt::Horizontal);
    tesselationSlider->setRange(0,5);
    tesselationSlider->setTickPosition(QSlider::TicksBothSides);
    tesselationSlider->setTickInterval(1);
    //connect(tesselationSlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setTesselation(int)));
    toolbar->addWidget(tesselationSlider);

    toolbar->addAction(cameraModeAction);
    toolbar->addAction(objectModeAction);
    toolbar->addAction(resetCameraAction);

    toolbar->addSeparator();

    viewDropButton = new QToolButton(this);
    viewDropButton->setMenu(viewMenu);
    viewDropButton->setPopupMode(QToolButton::InstantPopup);
    viewDropButton->setIcon(QIcon(":/img/viewports.png"));
    toolbar->addWidget(viewDropButton);

    toolbar->addSeparator();

    toolbar->addAction(addCubeAction);
    toolbar->addAction(addConeAction);
    toolbar->addAction(addCylinderAction);
    toolbar->addAction(addSphereAction);
    toolbar->addAction(addTorusAction);
    
    addToolBar(toolbar);
}

void MainWindow::createActions() {
    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(tr("Ctrl+5"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    
    aboutAction = new QAction("&About", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    resetCameraAction = new QAction("&Reset", this);
    resetCameraAction->setIcon(QIcon(":/img/cam_home.png"));
    connect(resetCameraAction, SIGNAL(triggered()), perspectiveGLWidget, SLOT(resetCamera()));
    connect(resetCameraAction, SIGNAL(triggered()), topGLWidget, SLOT(resetCamera()));
    connect(resetCameraAction, SIGNAL(triggered()), frontGLWidget, SLOT(resetCamera()));
    connect(resetCameraAction, SIGNAL(triggered()), rightGLWidget, SLOT(resetCamera()));

    singleViewAction = new QAction("&Single Viewport", this);
    singleViewAction->setShortcut(tr("Ctrl+1"));
    singleViewAction->setIcon(QIcon(":/img/view-single.png"));
    singleViewAction->setCheckable(true);
    connect(singleViewAction, SIGNAL(triggered()), this, SLOT(setSingleView()));

    dualViewAction = new QAction("&Dual Viewport", this);
    dualViewAction->setShortcut(tr("Ctrl+2"));
    dualViewAction->setIcon(QIcon(":/img/view-dual.png"));
    dualViewAction->setCheckable(true);
    connect(dualViewAction, SIGNAL(triggered()), this, SLOT(setDualView()));

    quadViewAction = new QAction("&Quad Viewports", this);
    quadViewAction->setShortcut(tr("Ctrl+4"));
    quadViewAction->setIcon(QIcon(":/img/viewports.png"));
    quadViewAction->setCheckable(true);
    connect(quadViewAction, SIGNAL(triggered()), this, SLOT(setQuadView()));

    viewportGroup = new QActionGroup(this);
    viewportGroup->addAction(singleViewAction);
    viewportGroup->addAction(dualViewAction);
    viewportGroup->addAction(quadViewAction);
    singleViewAction->setChecked(true);


    cameraModeAction = new QAction("Camera Mode", this);
    cameraModeAction->setIcon(QIcon(":/img/camera.png"));
    cameraModeAction->setCheckable(true);
    connect(cameraModeAction, SIGNAL(triggered()), this, SLOT(setCameraInteraction()));

    objectModeAction = new QAction("Object Mode", this);
    objectModeAction->setIcon(QIcon(":/img/select.png"));
    objectModeAction->setCheckable(true);
    connect(objectModeAction, SIGNAL(triggered()), this, SLOT(setObjectInteraction()));

    interactionGroup = new QActionGroup(this);
    interactionGroup->addAction(cameraModeAction);
    interactionGroup->addAction(objectModeAction);
    objectModeAction->setChecked(true);


    addCubeAction = new QAction(this);
    addCubeAction->setIcon(QIcon(":/img/box.png"));
    connect(addCubeAction, SIGNAL(triggered()), this, SLOT(addCube()));

    addCylinderAction = new QAction(this);
    addCylinderAction->setIcon(QIcon(":/img/cylinder.png"));
    connect(addCylinderAction, SIGNAL(triggered()), this, SLOT(addCylinder()));
    
    addSphereAction = new QAction(this);
    addSphereAction->setIcon(QIcon(":/img/sphere.png"));
    connect(addSphereAction, SIGNAL(triggered()), this, SLOT(addSphere()));
    
    addTorusAction = new QAction(this);
    addTorusAction->setIcon(QIcon(":/img/torus.png"));
    connect(addTorusAction, SIGNAL(triggered()), this, SLOT(addTorus()));
    
    addConeAction = new QAction(this);
    addConeAction->setIcon(QIcon(":/img/cone.png"));
    connect(addConeAction, SIGNAL(triggered()), this, SLOT(addCone()));
}

void MainWindow::createMenus() {
    menuBar = new QMenuBar();

    //Actions for the file menu
    fileMenu = new QMenu("&File");
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    //Viewports menu
    viewMenu = new QMenu("&View");
    viewMenu->addAction(singleViewAction);
    viewMenu->addAction(dualViewAction);
    viewMenu->addAction(quadViewAction);
    menuBar->addMenu(viewMenu);
    
    //Help menu
    helpMenu = new QMenu("&Help");
    helpMenu->addAction(aboutAction);
    menuBar->addMenu(helpMenu);

}


void MainWindow::addCube() {
    int tesselationLevel = 0;
    scene->addCube(currentNode, tesselationLevel);
    emit updateGL();
}

void MainWindow::addCone(){
    int tesselationLevel = 3;
    scene->addCone(currentNode, tesselationLevel);
    emit updateGL();
}
void MainWindow::addCylinder(){
    int tesselationLevel = 3;
    scene->addCylinder(currentNode, tesselationLevel);
    emit updateGL();
}
void MainWindow::addSphere(){
    int tesselationLevel = 3;
    scene->addSphere(currentNode, tesselationLevel);
    emit updateGL();
}
void MainWindow::addTorus(){
    int tesselationLevel = 3;
    scene->addTorus(currentNode, tesselationLevel);
    emit updateGL();
}


void MainWindow::showAboutBox() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Sebas!");
    msgBox.exec();
}

void MainWindow::setSingleView() {
    statusbar->showMessage("Setting single view", 2000);
    bottomSplitter->hide();
    frontGLWidget->hide();
}

void MainWindow::setDualView() {
    statusbar->showMessage("Setting dual view", 2000);
    bottomSplitter->hide();
    frontGLWidget->show();
}

void MainWindow::setQuadView() {
    statusbar->showMessage("Setting quad view", 2000);
    bottomSplitter->show();
    frontGLWidget->show();
}
	    
void MainWindow::setActiveViewport(GLWidget *active) {
    perspectiveGLWidget->setActive(false);
    topGLWidget->setActive(false);
    frontGLWidget->setActive(false);
    rightGLWidget->setActive(false);

    active->setActive(true);
}
	    
void MainWindow::setCameraInteraction() {
    perspectiveGLWidget->setCameraActive(true);
    topGLWidget->setCameraActive(true);
    frontGLWidget->setCameraActive(true);
    rightGLWidget->setCameraActive(true);

    statusbar->showMessage("Camera movement activate!", 2000);
}
	    
void MainWindow::setObjectInteraction() {
    perspectiveGLWidget->setCameraActive(false);
    topGLWidget->setCameraActive(false);
    frontGLWidget->setCameraActive(false);
    rightGLWidget->setCameraActive(false);
    
    statusbar->showMessage("Moving dem objects.", 2000);
}

void MainWindow::changeCurrentNode(const QModelIndex &current, const QModelIndex &previous) {
    this->currentNode = static_cast<SceneGraph*>(current.internalPointer());
    statusbar->showMessage(currentNode->getName().c_str(), 2000);
}

void MainWindow::translateNode(double x, double y, double z) {
    this->currentNode->translate(x, y, z);
    emit updateGL();
}

void MainWindow::rotateNode(QQuaternion *q) {
    this->currentNode->rotate(*q);
    emit updateGL();
}
