#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    menuBar = new QMenuBar();

    //Actions for the file menu
    fileMenu = new QMenu("&File");
    exitAction = new QAction("E&xit", fileMenu);
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    //Actions in the shading menu
    shadingMenu = new QMenu("&Shading");
    
    wireframeAction = new QAction("None (&Wireframe)", shadingMenu);
    wireframeAction->setShortcut(tr("Ctrl+1"));
    wireframeAction->setCheckable(true);
    shadingMenu->addAction(wireframeAction);
    
    flatAction = new QAction("&Flat", shadingMenu);
    flatAction->setShortcut(tr("Ctrl+2"));
    flatAction->setCheckable(true);
    shadingMenu->addAction(flatAction);
    
    gouraudAction = new QAction("&Gouraud", shadingMenu);
    gouraudAction->setShortcut(tr("Ctrl+3"));
    gouraudAction->setCheckable(true);
    shadingMenu->addAction(gouraudAction);
    
    phongAction = new QAction("&Phong", shadingMenu);
    phongAction->setShortcut(tr("Ctrl+4"));
    phongAction->setCheckable(true);
    shadingMenu->addAction(phongAction);

    shadingGroup = new QActionGroup(this);
    shadingGroup->addAction(wireframeAction);
    shadingGroup->addAction(flatAction);
    shadingGroup->addAction(gouraudAction);
    shadingGroup->addAction(phongAction);
    wireframeAction->setCheckable(true);
    menuBar->addMenu(shadingMenu);
    


    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{

}
