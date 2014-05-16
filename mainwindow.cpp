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
    setMinimumSize(700,700);


    //Set central OpenGL widget
    glWidgetContext = new GLWidgetContext();
    glWidgetContext->makeCurrent();
    glWidgetContext->initializeGL();

    
    Scene *scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation());
    scene->setLightLocation(glWidgetContext->getLightPositionLocation());
    
    perspectiveGLWidget = new GLWidget(this, glWidgetContext);
    perspectiveGLWidget->setPerspectiveCamera(1, 1, 3);
    perspectiveGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    perspectiveGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    perspectiveGLWidget->setScene(scene);
    
    frontGLWidget = new GLWidget(this, glWidgetContext);
    frontGLWidget->setOrthoCamera(0, 0, 3);
    frontGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    frontGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    frontGLWidget->setScene(scene);
    
    topGLWidget = new GLWidget(this, glWidgetContext);
    topGLWidget->setOrthoCamera(0, 3, 0);
    topGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    topGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    topGLWidget->setScene(scene);
    
    rightGLWidget = new GLWidget(this, glWidgetContext);
    rightGLWidget->setOrthoCamera(3, 0, 0);
    rightGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    rightGLWidget->setProjectionLocation(glWidgetContext->getPerspectiveMatLocation());
    rightGLWidget->setScene(scene);


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
    
    setCentralWidget(sideSplitter);

    sceneOutliner = new QTreeView();
    sceneOutliner->setWindowTitle(QObject::tr("Outliner"));
    sceneOutliner->setModel(scene);
    outlinerDock = new QDockWidget(tr("Scene Outliner"), this);
    outlinerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outlinerDock->setWidget(sceneOutliner);
    addDockWidget(Qt::LeftDockWidgetArea, outlinerDock);

    createActions();
    createMenus();
    createToolbar();
    
    //Status bar
    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);
    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

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

    dualViewAction = new QAction("&Dual Viewport", this);
    dualViewAction->setShortcut(tr("Ctrl+2"));
    dualViewAction->setIcon(QIcon(":/img/view-dual.png"));
    dualViewAction->setCheckable(true);

    quadViewAction = new QAction("&Quad Viewports", this);
    quadViewAction->setShortcut(tr("Ctrl+4"));
    quadViewAction->setIcon(QIcon(":/img/viewports.png"));
    quadViewAction->setCheckable(true);

    viewportGroup = new QActionGroup(this);
    viewportGroup->addAction(singleViewAction);
    viewportGroup->addAction(dualViewAction);
    viewportGroup->addAction(quadViewAction);
    singleViewAction->setChecked(true);
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


void MainWindow::showAboutBox() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Sebas!");
    msgBox.exec();
}
