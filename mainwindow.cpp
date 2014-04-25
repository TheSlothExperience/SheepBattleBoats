#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello Cube!!");
    setMinimumSize(500,500);


    //Set central OpenGL widget
    glwidget = new GLWidget(this);
    setCentralWidget(glwidget);

    createActions();
    createMenus();
    
    //Toolbar
    toolbar = new QToolBar(this);
    toolbar->addAction(wireframeAction);
    toolbar->addAction(flatAction);
    toolbar->addAction(gouraudAction);
    toolbar->addAction(phongAction);
    addToolBar(toolbar);

    //Status bar
    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);
    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

}


void MainWindow::createActions() {
    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    
    
    wireframeAction = new QAction("None (&Wireframe)", this);
    wireframeAction->setShortcut(tr("Ctrl+1"));
    wireframeAction->setCheckable(true);
    wireframeAction->setIcon(QIcon(":/img/wireframe.png"));
    connect(wireframeAction, SIGNAL(triggered()), glwidget, SLOT(setWireframeShading()));
    
    flatAction = new QAction("&Flat", this);
    flatAction->setShortcut(tr("Ctrl+2"));
    flatAction->setCheckable(true);
    flatAction->setIcon(QIcon(":/img/flat.png"));
    connect(flatAction, SIGNAL(triggered()), glwidget, SLOT(setFlatShading()));
    
    gouraudAction = new QAction("&Gouraud", this);
    gouraudAction->setShortcut(tr("Ctrl+3"));
    gouraudAction->setCheckable(true);
    gouraudAction->setIcon(QIcon(":/img/gouraud.png"));
    connect(gouraudAction, SIGNAL(triggered()), glwidget, SLOT(setGouraudShading()));
    
    phongAction = new QAction("&Phong", this);
    phongAction->setShortcut(tr("Ctrl+4"));
    phongAction->setCheckable(true);
    phongAction->setIcon(QIcon(":/img/phong.png"));
    

    shadingGroup = new QActionGroup(this);
    shadingGroup->addAction(wireframeAction);
    shadingGroup->addAction(flatAction);
    shadingGroup->addAction(gouraudAction);
    shadingGroup->addAction(phongAction);
    flatAction->setChecked(true);

    
    aboutAction = new QAction("&About", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
}

void MainWindow::createMenus() {
    menuBar = new QMenuBar();

    //Actions for the file menu
    fileMenu = new QMenu("&File");
    fileMenu->addAction(exitAction);
    fileMenu->addAction(aboutAction);
    menuBar->addMenu(fileMenu);

    //Actions in the shading menu
    shadingMenu = new QMenu("&Shading");
    shadingMenu->addAction(wireframeAction);
    shadingMenu->addAction(flatAction);
    shadingMenu->addAction(gouraudAction);
    shadingMenu->addAction(phongAction);
    menuBar->addMenu(shadingMenu);
}


void MainWindow::showAboutBox() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Sebas!");
    msgBox.exec();
}
