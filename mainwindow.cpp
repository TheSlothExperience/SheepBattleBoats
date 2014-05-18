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

    createActions();
    createMenus();
    createToolbar();
    
    setupGL();

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

void MainWindow::setupGL() {
    //Set central OpenGL widget
    glWidgetContext = new GLWidgetContext();
    glWidgetContext->makeCurrent();
    glWidgetContext->initializeGL();

    
    scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation());
    scene->setLightLocation(glWidgetContext->getLightPositionLocation());
    currentNode = scene->root();
    addCube();
    
    perspectiveGLWidget = new GLWidget(this, glWidgetContext);
    perspectiveGLWidget->setPerspectiveCamera(1, 1, 3);
    perspectiveGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    perspectiveGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    perspectiveGLWidget->setScene(scene);
    
    connect(perspectiveGLWidget, SIGNAL(translate(double, double, double)), this, SLOT(translateNode(double, double, double)));
    connect(perspectiveGLWidget, SIGNAL(rotate(QQuaternion*)), this, SLOT(rotateNode(QQuaternion*)));
    connect(this, SIGNAL(updateGL()), perspectiveGLWidget, SLOT(forceGLupdate()));
    
    frontGLWidget = new GLWidget(this, glWidgetContext);
    frontGLWidget->setOrthoCamera(0, 0, 3);
    frontGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    frontGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    frontGLWidget->setScene(scene);
    
    connect(frontGLWidget, SIGNAL(translate(double, double, double)), this, SLOT(translateNode(double, double, double)));
    connect(frontGLWidget, SIGNAL(rotate(QQuaternion*)), this, SLOT(rotateNode(QQuaternion*)));
    connect(this, SIGNAL(updateGL()), frontGLWidget, SLOT(forceGLupdate()));
    
    topGLWidget = new GLWidget(this, glWidgetContext);
    topGLWidget->setOrthoCamera(0, 3, 0);
    topGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    topGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    topGLWidget->setScene(scene);
    
    connect(topGLWidget, SIGNAL(translate(double, double, double)), this, SLOT(translateNode(double, double, double)));
    connect(topGLWidget, SIGNAL(rotate(QQuaternion*)), this, SLOT(rotateNode(QQuaternion*)));
    connect(this, SIGNAL(updateGL()), topGLWidget, SLOT(forceGLupdate()));
    
    rightGLWidget = new GLWidget(this, glWidgetContext);
    rightGLWidget->setOrthoCamera(3, 0, 0);
    rightGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    rightGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    rightGLWidget->setScene(scene);
    
    connect(rightGLWidget, SIGNAL(translate(double, double, double)), this, SLOT(translateNode(double, double, double)));
    connect(rightGLWidget, SIGNAL(rotate(QQuaternion*)), this, SLOT(rotateNode(QQuaternion*)));
    connect(this, SIGNAL(updateGL()), rightGLWidget, SLOT(forceGLupdate()));


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
    //connect(resetCameraAction, SIGNAL(triggered()), glwidget, SLOT(resetCamera()));

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
void MainWindow::addTorus(){}


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
