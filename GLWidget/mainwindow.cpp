#include "mainwindow.h"
#include "glwidget.h"
#include "scene.h"
#include "camera.h"
#include "perspectivecamera.h"
#include "orthocamera.h"

#include <QDataStream>
#include <QFile>
#include <QImage>
#include <iostream>
#include <cstdio>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("Hello Cube!!");
	setMinimumSize(1000,800);

	//Status bar
	statusbar = new QStatusBar(this);

	setupGL();

	createActions();
	createMenus();
	createToolbar();

	createColorDock();

	setStatusBar(statusbar);
	setMenuBar(menuBar);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

    initGameLogic();
    for(int i=0;i<100;i++){
        qDebug()<<(rand()%(100))/100.0;
    }
}


void MainWindow::initGameLogic(){

    initLevel();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(gameTick()));
    timer->start(34);


//    for(int i=0;i<scene->getLvlObjAdresses().length();i++){
//        qDebug()<<"lvlObj"+QString::number(i);
//    }
}

void MainWindow::gameTick(){
    testCollisions();
    doMovements();
    if(shooting){
        shootingLatency++;
        if(shootingLatency>50){
            shooting=false;
        }
    }
    scene->behaviourExecutions();
    emit updateGL();
}

void MainWindow::testCollisions(){
    scene->testCollisions();
}

void MainWindow::doMovements(){

    //"Vor und zurück"
    if(wPressed){
        scene->getMainBoat()->increaseVelocity(QVector3D(0.0,0.0,-0.002));

    }else if(sPressed){
        if(scene->getMainBoat()->getVelocity().z()<0){
            scene->getMainBoat()->increaseVelocity(QVector3D(0.0,0.0,0.004));
        }else{
            scene->getMainBoat()->increaseVelocity(QVector3D(0.0,0.0,0.001));
        }
    }else{
        scene->getMainBoat()->stopVelocity();
    }

    if(dPressed){
        scene->getMainBoat()->changeSteeringValue(0.02);
    }else if(aPressed){
        scene->getMainBoat()->changeSteeringValue(-0.02);
    }else{
        scene->getMainBoat()->stopRotating();
    }


    QVector3D newTranslation= scene->convertToMotherSheepTranslation();
    scene->translateMotherSheep(newTranslation);
    scene->rotateMotherSheep();
    activeViewport->translateBoardCamera(newTranslation);
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

	//Create the widgets
	perspectiveGLWidget = new GLWidget(this, glWidgetContext);
	widgetList.push_back(perspectiveGLWidget);
    perspectiveGLWidget->setPerspectiveCamera(0, 1, 5);

	frontGLWidget = new GLWidget(this, glWidgetContext);
	widgetList.push_back(frontGLWidget);
	frontGLWidget->setOrthoCamera(0, 0, 3);

	topGLWidget = new GLWidget(this, glWidgetContext);
	widgetList.push_back(topGLWidget);
	topGLWidget->setOrthoCamera(0, 3, 0);

	rightGLWidget = new GLWidget(this, glWidgetContext);
	widgetList.push_back(rightGLWidget);
	rightGLWidget->setOrthoCamera(3, 0, 0);

	perspectiveGLWidget->makeCurrent();
	perspectiveGLWidget->initializeGL();


	//Set the scene
	GLuint idLocation = glWidgetContext->getShaders().shaderProgram->uniformLocation("id");
	GLuint colorLocation = glWidgetContext->getShaders().shaderProgram->uniformLocation("color");
	scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation(), idLocation, colorLocation);
	scene->setLightLocation(glWidgetContext->getLightPositionLocation());

	sceneOutliner = new QTreeView();
	sceneOutliner->setWindowTitle(QObject::tr("Outliner"));
	sceneOutliner->setModel(scene);
	sceneOutliner->setDragEnabled(true);
	sceneOutliner->setAcceptDrops(true);
	sceneOutliner->setDragDropMode(QAbstractItemView::InternalMove);
	sceneOutliner->setDropIndicatorShown(true);

	connect(sceneOutliner->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(changeCurrentNode(const QModelIndex&, const QModelIndex&)));

	outlinerDock = new QDockWidget(tr("Scene Outliner"), this);
	outlinerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	outlinerDock->setWidget(sceneOutliner);
	addDockWidget(Qt::LeftDockWidgetArea, outlinerDock);

	currentNode = scene->root();


	//Map over the widgets setting the scene and connecting the signals
	mapWidgets([=](GLWidget *w){w->setScene(scene);});
	mapWidgets([=](GLWidget *w){w->setShaders(glWidgetContext->getShaders());});

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
	mapWidgets([=](GLWidget *w){
			connect(w, SIGNAL(changedActiveId(int)), this, SLOT(changeActiveId(int)));
		});
	mapWidgets([=](GLWidget *w){
			connect(w, SIGNAL(changedCurrent(QModelIndex)), this, SLOT(changeCurrent(QModelIndex)));
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
	activeViewport = perspectiveGLWidget;

	setCentralWidget(sideSplitter);
}

void MainWindow::createColorDock() {
	colorDock = new QDockWidget(tr("Colour Selector of Doom"), this);
	colorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |
	                           Qt::BottomDockWidgetArea);

	redSlider = new QSlider(Qt::Horizontal, this);
	redSlider->setRange(0,255);
	redSlider->setValue(255);
	connect(redSlider, SIGNAL(valueChanged(int)), this, SLOT(changedColor()));

	greenSlider = new QSlider(Qt::Horizontal,this);
	greenSlider->setRange(0,255);
	greenSlider->setValue(255);
	connect(greenSlider, SIGNAL(valueChanged(int)), this, SLOT(changedColor()));

	blueSlider = new QSlider(Qt::Horizontal,this);
	blueSlider->setRange(0,255);
	blueSlider->setValue(255);
	connect(blueSlider, SIGNAL(valueChanged(int)), this, SLOT(changedColor()));

	satShadowCheckBox = new QCheckBox("SAT shadows");
	connect(satShadowCheckBox, SIGNAL(toggled(bool)), this, SLOT(setSATShadows(bool)));


	QWidget *contents = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(contents);
	layout->addWidget(satShadowCheckBox);
	layout->addWidget(redSlider);
	layout->addWidget(greenSlider);
	layout->addWidget(blueSlider);
	colorDock->setWidget(contents);
	addDockWidget(Qt::LeftDockWidgetArea, colorDock);
}

void MainWindow::createToolbar() {
	//Toolbar
	toolbar = new QToolBar(this);

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
	toolbar->addAction(addGroupAction);
    toolbar->addAction(add3DModelAction);

	toolbar->addSeparator();
	toolbar->addAction(addLightAction);

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
	connect(addConeAction, SIGNAL(triggered()), this, SLOT(addCone()));;

	addGroupAction = new QAction(this);
    addGroupAction->setIcon(QIcon(":/img/group.png"));
	connect(addGroupAction, SIGNAL(triggered()), this, SLOT(addGroup()));;

	addLightAction = new QAction(this);
    addLightAction->setIcon(QIcon(":/img/light.png"));
	connect(addLightAction, SIGNAL(triggered()), this, SLOT(addLight()));

    add3DModelAction = new QAction(this);
    add3DModelAction->setIcon(QIcon(":/img/add.png"));
    connect(add3DModelAction, SIGNAL(triggered()),this,SLOT(add3DModel()));

//    addLvlObjAction = new QAction(this);
//    addLvlObjAction ->setIcon(QIcon(":/img/add.png"));
//    connect(addLvlObjAction , SIGNAL(triggered()),this,SLOT(addLvlObj()));

//    shootAction=new QAction(this);
//    shootAction->setShortcut(tr("Space"));
//    connect(shootAction,SIGNAL(triggered()),this,SLOT(shoot()));
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

void MainWindow::initLevel(){
    scene->initLevel();
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete) {
        //Delete the current node
        QModelIndex idx = sceneOutliner->selectionModel()->currentIndex();
        QString msg;
        if(static_cast<SceneGraph*>(idx.internalPointer()) == scene->root()) {
            statusbar->showMessage("Cannot delete the root, fool!", 5000);
        } else {
            msg += "Deleting: ";
            msg += scene->data(idx, Qt::DisplayRole).toString();
            statusbar->showMessage(msg, 5000);
            if(scene->removeRows(idx.row(), 1, idx.parent())) {
                emit updateGL();
            }
        }
    } else if(event->key() == Qt::Key_Control) {
        //Toggle camera and interactive mode
        if(cameraModeAction->isChecked()) {
            objectModeAction->setChecked(true);
            objectModeAction->activate(QAction::Trigger);
		} else {
			cameraModeAction->setChecked(true);
			cameraModeAction->activate(QAction::Trigger);
		}
    }else if(event->key() == Qt::Key_W) {
        wPressed=true;
    }  else if(event->key() == Qt::Key_S) {
        sPressed=true;
    } else if(event->key() == Qt::Key_A) {
        dPressed=true;
    } else if(event->key() == Qt::Key_D) {
        aPressed=true;
    }else if(event->key() == Qt::Key_Space) {
//        shootingHeight=0.0;
//        shooting=true;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Control) {
		//Toggle camera and interactive mode
		if(cameraModeAction->isChecked()) {
			objectModeAction->setChecked(true);
			objectModeAction->activate(QAction::Trigger);
		} else {
			cameraModeAction->setChecked(true);
			cameraModeAction->activate(QAction::Trigger);
		}
    }else if(event->key() == Qt::Key_W) {
        wPressed=false;
    }  else if(event->key() == Qt::Key_S) {
        sPressed=false;
    } else if(event->key() == Qt::Key_A) {
        dPressed=false;
    } else if(event->key() == Qt::Key_D) {
        aPressed=false;
    }else if(event->key() == Qt::Key_Space) {
//        shooting=false;
        shoot();
    }


}


void MainWindow::addCube() {
	int tesselationLevel = 0;
	QModelIndex idx = scene->addCube(currentNode, tesselationLevel);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addCone(){
    QModelIndex idx = scene->addCone(currentNode, 10);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addCylinder(){
    QModelIndex idx = scene->addCylinder(currentNode, 10);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addSphere(){
    QModelIndex idx = scene->addSphere(currentNode, 6);
    sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
    emit updateGL();
}
void MainWindow::addTorus(){
    QModelIndex idx = scene->addTorus(currentNode, 6);
    sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
    emit updateGL();
}

void MainWindow::addGroup(){
    QModelIndex idx = scene->addGroup(currentNode);
    sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}

void MainWindow::addLight(){
	QModelIndex idx = scene->addLight();
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}

void MainWindow::add3DModel(){
    load3DModel();
    QModelIndex idx = scene->add3DModel(currentNode);
    sceneOutliner->selectionModel()->setCurrentIndex(idx,QItemSelectionModel::Current | QItemSelectionModel::Select);
    emit updateGL();
}

//void MainWindow::addLvlObj(){
//    QModelIndex idx = scene->add3DModel(currentNode);
//    sceneOutliner->selectionModel()->setCurrentIndex(idx,QItemSelectionModel::Current | QItemSelectionModel::Select);
//    emit updateGL();
//}

void MainWindow::changeActiveId(int id){
	mapWidgets([=](GLWidget *w){w->changeActiveId(id);});
}

void MainWindow::setSATShadows(bool set){
	mapWidgets([=](GLWidget *w){w->setSATShadows(set);});
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
	setActiveViewport(perspectiveGLWidget);
}

void MainWindow::setDualView() {
	statusbar->showMessage("Setting dual view", 2000);
	bottomSplitter->hide();
	frontGLWidget->show();
	//Switch to first VP, except if the second one was
	//already active.
	if(!frontGLWidget->getActive()) {
		setActiveViewport(perspectiveGLWidget);
	}
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

	activeViewport = active;
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


void MainWindow::changeCurrentNode(const QModelIndex &current, const QModelIndex &) {
	this->currentNode = static_cast<SceneGraph*>(current.internalPointer());
	changeActiveId(currentNode->getId());
	statusbar->showMessage(currentNode->getName().c_str(), 2000);
}

void MainWindow::changeCurrent(QModelIndex q){
	sceneOutliner->selectionModel()->setCurrentIndex(q, QItemSelectionModel::Current | QItemSelectionModel::Select);
	//Also get the colors!
	SceneGraph *s = static_cast<SceneGraph*>(q.internalPointer());
	GLfloat *color = s->getColor();
	redSlider->setValue(color[0] * 255);
	greenSlider->setValue(color[1] * 255);
	blueSlider->setValue(color[2] * 255);
	statusbar->showMessage(s->getName().c_str(), 2000);
}

void MainWindow::translateNode(double x, double y, double z) {
	this->currentNode->translate(x, y, z);
	emit updateGL();
}

void MainWindow::rotateNode(QQuaternion *q) {
	this->currentNode->rotate(*q);
	emit updateGL();
}

void MainWindow::changedColor() {
	this->currentNode->changeColor(redSlider->sliderPosition() / (float) 255, greenSlider->sliderPosition() / (float) 255, blueSlider->sliderPosition() / (float) 255, 1.0f);
	emit updateGL();
}

void MainWindow::load3DModel(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load 3D Model"), ".", tr("test(*.obj)"));
    QFileInfo info(fileName);
    std::cout << "File: " << fileName.toLocal8Bit().data() << std::endl;
    if(!fileName.isEmpty()) {
        FILE *fp;
        fp = fopen(fileName.toLocal8Bit().data(), "r");



        fclose(fp);
        emit updateGL();
    }

}

void MainWindow::shoot(){
    qDebug()<<"shoot";
    if(!shooting){
        shooting=true;
        shootingLatency=0;
        QQuaternion rot = scene->getMainBoat()->getRotation();
        QVector3D temp=QVector3D(0.0,5.0,-4.0);
        temp=rot.rotatedVector(temp);
        scene->addProjectile(temp);
    }

}
