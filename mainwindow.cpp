#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    
    setWindowTitle("Hello Cube!!");
    setMinimumSize(500,500);

    
    menuBar = new QMenuBar();

    //Actions for the file menu
    fileMenu = new QMenu("&File");
    
    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    //Actions in the shading menu
    shadingMenu = new QMenu("&Shading");
    
    wireframeAction = new QAction("None (&Wireframe)", shadingMenu);
    wireframeAction->setShortcut(tr("Ctrl+1"));
    wireframeAction->setCheckable(true);
    wireframeAction->setIcon(QIcon(":/img/wireframe.png"));
    shadingMenu->addAction(wireframeAction);
    
    flatAction = new QAction("&Flat", shadingMenu);
    flatAction->setShortcut(tr("Ctrl+2"));
    flatAction->setCheckable(true);
    flatAction->setIcon(QIcon(":/img/flat.png"));
    shadingMenu->addAction(flatAction);
    
    gouraudAction = new QAction("&Gouraud", shadingMenu);
    gouraudAction->setShortcut(tr("Ctrl+3"));
    gouraudAction->setCheckable(true);
    gouraudAction->setIcon(QIcon(":/img/gouraud.png"));
    shadingMenu->addAction(gouraudAction);
    
    phongAction = new QAction("&Phong", shadingMenu);
    phongAction->setShortcut(tr("Ctrl+4"));
    phongAction->setCheckable(true);
    phongAction->setIcon(QIcon(":/img/phong.png"));
    shadingMenu->addAction(phongAction);

    shadingGroup = new QActionGroup(this);
    shadingGroup->addAction(wireframeAction);
    shadingGroup->addAction(flatAction);
    shadingGroup->addAction(gouraudAction);
    shadingGroup->addAction(phongAction);
    flatAction->setChecked(true);

    menuBar->addMenu(shadingMenu);
    
    //About section
    aboutAction = new QAction("&About", menuBar);
    fileMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    //Toolbar
    toolbar = new QToolBar(this);
    toolbar->setGeometry(0, 0, 500, 40);
    toolbar->addAction(wireframeAction);
    toolbar->addAction(flatAction);
    toolbar->addAction(gouraudAction);
    toolbar->addAction(phongAction);

    //Status bar
    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);

    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

}


void MainWindow::showAboutBox() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Sebas!");
    msgBox.exec();
}
