#include "mainwindow.h"
#include "glwidget.h"
#include <qapplication.h>
#include "scene.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello Cube!!");
    setMinimumSize(500,500);


    //Set central OpenGL widget
    glWidgetContext = new GLWidgetContext();
    glWidgetContext->makeCurrent();
    glWidgetContext->initializeGL();

    
    Scene *scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation());
    scene->setLightLocation(glWidgetContext->getLightPositionLocation());
    
    perspectiveGLWidget = new GLWidget(this, glWidgetContext);
    perspectiveGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());
    

    perspectiveGLWidget->setScene(scene);
    // topGLWidget = new GLWidget(this, glWidgetContext);
    // topGLWidget->setScene(scene);
    // topGLWidget->setShaderProgram(glWidgetContext->getShaderProgram());

    // topSplitter = new QSplitter(this);
    // topSplitter->addWidget(perspectiveGLWidget);
    // topSplitter->addWidget(topGLWidget);
    
    // setCentralWidget(topSplitter);
    setCentralWidget(perspectiveGLWidget);
    //setCentralWidget(glWidgetContext);

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
