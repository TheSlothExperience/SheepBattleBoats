#include "mainwindow.h"
#include "glwidget.h"
#include "scene.h"
#include "camera.h"
#include "perspectivecamera.h"
#include "orthocamera.h"
#include "multislider.h"

#include <QDataStream>
#include <QFile>
#include <QImage>
#include <iostream>
#include <cstdio>

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

	createIsoValuer();
	createTfEditor();

	createHeightMapDock();

	createColorDock();

	setStatusBar(statusbar);
	setMenuBar(menuBar);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();
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
	GLuint idLocation = glWidgetContext->getShaderProgram()->uniformLocation("id");
	scene = new Scene(glWidgetContext->getModelViewMatLocation(), glWidgetContext->getNormalMatLocation(), idLocation);
	scene->setLightLocation(glWidgetContext->getLightPositionLocation());
	scene->setShaderProgram(glWidgetContext->getShaderProgram());


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
	mapWidgets([=](GLWidget *w){w->setCanvasProgram(glWidgetContext->getCanvasProgram());});
	mapWidgets([=](GLWidget *w){w->setVolumeProgram(glWidgetContext->getVolumeProgram());});
	mapWidgets([=](GLWidget *w){w->setQuadViewProgram(glWidgetContext->getQuadViewProgram());});
	mapWidgets([=](GLWidget *w){w->setHeightMapProgram(glWidgetContext->getHeightMapProgram());});

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

void MainWindow::createTfEditor() {
	tfeditor = new TfEditor();
	connect(tfeditor, SIGNAL(tfChanged()), this, SLOT(changeTF()));
	addDockWidget(Qt::RightDockWidgetArea, tfeditor);
}

void MainWindow::createHeightMapDock() {
	heightMapLoadButton = new QPushButton("Load heightmap");
	connect(heightMapLoadButton, SIGNAL(clicked()), this, SLOT(loadHeightMap()));

	factureLoadButton = new QPushButton("Load facture");
	connect(factureLoadButton, SIGNAL(clicked()), this, SLOT(loadFacture()));

	showMeshCheckBox = new QCheckBox("Show Mesh");
	connect(showMeshCheckBox, SIGNAL(toggled(bool)), this, SLOT(showMesh(bool)));

	heightMapDock = new QDockWidget(tr("Terranizer Originator Ultra"), this);
	heightMapDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |
	                         Qt::BottomDockWidgetArea);


	QWidget *contents = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(contents);

	layout->addWidget(showMeshCheckBox);
	layout->addWidget(heightMapLoadButton);
	layout->addWidget(factureLoadButton);

	heightMapDock->setWidget(contents);
	addDockWidget(Qt::RightDockWidgetArea, heightMapDock);
}

void MainWindow::createIsoValuer() {
	isovalueSlider = new QSlider(Qt::Horizontal, this);
	isovalueSlider->setRange(0, 255);
	isovalueSlider->setValue(100);
	connect(isovalueSlider, SIGNAL(valueChanged(int)), this, SLOT(changeIsovalue(int)));

	isovalueCheckBox = new QCheckBox("IsoSurface");
	isovalueCheckBox->setChecked(false);
	connect(isovalueCheckBox, SIGNAL(toggled(bool)), this, SLOT(showIsoSurface(bool)));

	isovalueAlpha = new QSlider(Qt::Horizontal, this);
	isovalueAlpha->setRange(0, 255);
	isovalueAlpha->setValue(255);
	connect(isovalueAlpha, SIGNAL(valueChanged(int)), this, SLOT(changeIsoAlpha(int)));

	isovalueAlphaLabel = new QLabel("Opacity: ", this);

	isoDock = new QDockWidget(tr("IsoSurface Selector Quantizer"), this);
	isoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |
	                         Qt::BottomDockWidgetArea);

	QWidget *contents = new QWidget;
	QGridLayout *layout = new QGridLayout(contents);
	layout->addWidget(isovalueCheckBox, 0, 0);
	layout->addWidget(isovalueSlider, 0, 1);
	layout->addWidget(isovalueAlphaLabel, 1, 0);
	layout->addWidget(isovalueAlpha, 1, 1);
	isoDock->setWidget(contents);
	addDockWidget(Qt::LeftDockWidgetArea, isoDock);
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

	QWidget *contents = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(contents);
	layout->addWidget(redSlider);
	layout->addWidget(greenSlider);
	layout->addWidget(blueSlider);
	colorDock->setWidget(contents);
	addDockWidget(Qt::LeftDockWidgetArea, colorDock);
}

void MainWindow::createToolbar() {
	//Toolbar
	toolbar = new QToolBar(this);

	tesselationSlider = new QSlider(Qt::Horizontal);
	tesselationSlider->setRange(1,8);
	tesselationSlider->setTickPosition(QSlider::TicksBothSides);
	tesselationSlider->setTickInterval(1);
	tesselationSlider->setValue(3);
	tesselationLevel = 3;
	connect(tesselationSlider, SIGNAL(valueChanged(int)), this, SLOT(setTesselation(int)));
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
	toolbar->addAction(addGroupAction);

	toolbar->addSeparator();
	toolbar->addAction(loadVolumeDataAction);

	toolbar->addSeparator();
	toolbar->addAction(addLightAction);

	addToolBar(toolbar);
}

void MainWindow::createActions() {
	exitAction = new QAction("E&xit", this);
	exitAction->setShortcut(tr("Ctrl+5"));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	loadVolumeDataAction = new QAction("&Load volumetric data", this);
	loadVolumeDataAction->setShortcut(Qt::CTRL + Qt::Key_L);
	connect(loadVolumeDataAction, SIGNAL(triggered()), this, SLOT(loadVolumeData()));


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
}

void MainWindow::createMenus() {
	menuBar = new QMenuBar();

	//Actions for the file menu
	fileMenu = new QMenu("&File");
	fileMenu->addAction(exitAction);
	fileMenu->addAction(loadVolumeDataAction);
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
	} else if(event->key() == Qt::Key_W) {
		activeViewport->translateCamera(0, 0, -0.1);
		QString msg = "Camera world position: ";
		QVector3D cPos = activeViewport->getCameraWorldPosition();
		msg += QString::number(cPos.x());
		msg += ", ";
		msg += QString::number(cPos.y());
		msg += ", ";
		msg += QString::number(cPos.z());
		float height = scene->heightMap()->getHeightAt(cPos.x(), cPos.z());
		msg += ". With height: " + QString::number(height);
		statusbar->showMessage(msg, 5000);
		emit updateGL();
	} else if(event->key() == Qt::Key_S) {
		activeViewport->translateCamera(0, 0, 0.1);
		emit updateGL();
	} else if(event->key() == Qt::Key_A) {
		activeViewport->rotateCamera(-0.03);
		emit updateGL();
	} else if(event->key() == Qt::Key_D) {
		activeViewport->rotateCamera(0.03);
		emit updateGL();
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
	}
}


void MainWindow::addCube() {
	int tesselationLevel = 0;
	QModelIndex idx = scene->addCube(currentNode, tesselationLevel);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}

void MainWindow::addCone(){
	QModelIndex idx = scene->addCone(currentNode, tesselationLevel);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addCylinder(){
	QModelIndex idx = scene->addCylinder(currentNode, tesselationLevel);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addSphere(){
	QModelIndex idx = scene->addSphere(currentNode, tesselationLevel);
	sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
	emit updateGL();
}
void MainWindow::addTorus(){
	QModelIndex idx = scene->addTorus(currentNode, tesselationLevel);
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

void MainWindow::changeActiveId(int id){
	mapWidgets([=](GLWidget *w){w->changeActiveId(id);});
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

void MainWindow::setTesselation(int t) {
	this->tesselationLevel = t;
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

void MainWindow::showIsoSurface(bool show) {
	if(scene->hasVolume()) {
		this->scene->volume()->setIso(show);
		emit updateGL();
	}
}

void MainWindow::changeIsovalue(int value) {
	if(scene->hasVolume()) {
		this->scene->volume()->setIsoValue(value);
		emit updateGL();
	}
}

void MainWindow::changeIsoAlpha(int value) {
	if(scene->hasVolume()) {
		this->scene->volume()->setIsoAlpha(value);
		emit updateGL();
	}
}

void MainWindow::changedColor() {
	this->currentNode->changeColor(redSlider->sliderPosition() / (float) 255, greenSlider->sliderPosition() / (float) 255, blueSlider->sliderPosition() / (float) 255, 1.0f);
	emit updateGL();
}

void MainWindow::changeTF() {
	if(scene->hasVolume()) {
		this->scene->volume()->changeTF(tfeditor->tfDisplay.getTF());
		this->scene->volume()->setMIP(tfeditor->getMIP());
		emit updateGL();
	}
}

void MainWindow::loadVolumeData() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load volumetric data"), ".", tr("Volume Data (*.raw)"));
	QFileInfo info(fileName);
	std::cout << "File: " << fileName.toLocal8Bit().data() << std::endl;
	if(!fileName.isEmpty()) {
		int x, y, z;
		float ax, ay, az;
		QFile f(fileName);

		//If there is no volume, add a volume first
		if(!scene->hasVolume()) {
			QModelIndex idx = scene->addVolume();
			sceneOutliner->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Current | QItemSelectionModel::Select);
		}

		if(f.open(QIODevice::ReadOnly)) {
			QTextStream prelude(&f);
			prelude >> x >> y >> z >> ax >> ay >> az;
			prelude.readLine();

			if(info.baseName() == QString("Tooth")) {
				unsigned short *raw = new unsigned short[x*y*z];

				//QTextStream is weird...don't delete this line.
				f.pos(); //Reseek the position
				prelude.pos();

				FILE* file = fdopen(f.handle(), "r");
				int readError = fread((void *)raw, 2, x*y*z, file);
				if(readError < 0) {
					std::cout << "Error reading volume data from file!" << std::endl;
					fclose(file);
					return;
				}

				std::cout << "Loading tooth texture with: "
				          << "(" << x << ", " << y << ", " << z << ")"
				          << " and aspect ratios of: "
				          << ax << " " << ay << " " << az << std::endl;


				//Calculate histogram
				int histogram_acc[256];
				for(int i = 0; i < 256; i++) {
					histogram_acc[i] = 0;
				}
				for(int i = 1; i < x*y*z; i++) {
					int idx = (float) raw[i] * (255.0 / 65536.0);
					histogram_acc[idx] += 1;
				}
				int m = 0;
				for(int i = 1; i < 256; i++) {
					if(histogram_acc[i] > m) {
						m = histogram_acc[i];
					}
				}
				float scale = 255.0 / (float) m;
				//Normalize histogram
				unsigned char histogram[256];
				for(int i = 0; i < 256; i++) {
					histogram[i] = (unsigned char)((float)histogram_acc[i] * scale);
				}
				//Send the raw data to the texture
				this->scene->loadVolumeData(x, y, z, ax, ay, az, raw);
				tfeditor->updateHistogram(histogram);
				fclose(file);

			} else {
				unsigned char *raw = new unsigned char[x*y*z];
				QDataStream rawData(&f);
				//QTextStream is weird...don't delete this line.
				prelude.pos();

				//Read into the char array
				rawData.readRawData((char *)raw, x*y*z);
				std::cout << "Loading texture with: "
				          << "(" << x << ", " << y << ", " << z << ")"
				          << " and aspect ratios of: "
				          << ax << " " << ay << " " << az << std::endl;


				//Calculate histogram
				int histogram_acc[256];
				for(int i = 0; i < 256; i++) {
					histogram_acc[i] = 0;
				}
				for(int i = 0; i < x*y*z; i++) {
					histogram_acc[(int)raw[i]] += 1;
				}
				int m = 0;
				for(int i = 1; i < 256; i++) {
					if(histogram_acc[i] > m) {
						m = histogram_acc[i];
					}
				}
				float scale = 255.0 / (float) m;
				//Normalize histogram
				unsigned char histogram[256];
				for(int i = 1; i < 256; i++) {
					histogram[i] = (unsigned char)((float)histogram_acc[i] * scale);
				}
				//Send the raw data to the texture
				this->scene->loadVolumeData(x, y, z, ax, ay, az, raw);
				tfeditor->updateHistogram(histogram);
			}
			f.close();
			emit updateGL();
		}
	}
}

void MainWindow::loadHeightMap() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load height map"), ".", tr("Height Map (*.pgm)"));
	QFileInfo info(fileName);
	std::cout << "File: " << fileName.toLocal8Bit().data() << std::endl;
	if(!fileName.isEmpty()) {
		FILE *fp;
		fp = fopen(fileName.toLocal8Bit().data(), "r");
		int height, width, valueRange;
		int ferr = fscanf(fp, "P5\n");
		if(ferr != 0) std::cout << "Problem reading file. File is evil." << std::endl;
		ferr = fscanf(fp, "%d\n", &width);
		if(ferr != 1) std::cout << "Problem reading file. File is evil." << std::endl;
		ferr = fscanf(fp, "%d\n", &height);
		if(ferr != 1) std::cout << "Problem reading file. File is evil." << std::endl;
		ferr = fscanf(fp, "%d\n", &valueRange);
		if(ferr != 1) std::cout << "Problem reading file. File is evil." << std::endl;

		if(valueRange < 256) {

		} else if (valueRange < 65536) {
			std::cout << "Loading height map texture with: "
			          << "(" << width << ", " << height << ")"
			          << " and maximum value of: "
			          << valueRange << " `short`-wise ." << std::endl;

			unsigned short* raw = new unsigned short[width * height];
			int read = 0;
			unsigned char hi, lo;

			//For some ungodly reason the endianness of PGM and
			//x86 is not the same. That's the reason for these
			//incantations.
			unsigned short val = 0;
			unsigned short max = 0;
			while(true) {
				hi = fgetc(fp);
				lo = fgetc(fp);
				val = (hi << 8) + lo;
				raw[read] = val;
				if(val > max && val < 65535) max = val;
				if(feof(fp)) break;
				read++;
			}
			if(read != width * height) {
				std::cout << "Uh-oh. Could only read " << read
				          << " wingamajings from the file, instead of "
				          << width * height << ". "
				          << "I recommend exorcising the file." << std::endl;
				fclose(fp);
				return;
			}
			scene->loadHeightMapData(width, height, raw);
			scene->heightMap()->setMaximumValue(max);
		}
		fclose(fp);
		emit updateGL();
	}
}

void MainWindow::loadFacture() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load facture"), ".", tr("Facture Texture (*.jpg)"));
	QFileInfo info(fileName);
	std::cout << "File: " << fileName.toLocal8Bit().data() << std::endl;
	if(!fileName.isEmpty()) {
		QImage facture;
		facture.load(fileName);
		std::cout << "Loading facture texture with: "
		          << "(" << facture.width() << ", " << facture.height() << ")" <<  std::endl;

		facture = QGLWidget::convertToGLFormat(facture);

		if(scene->hasHeightMap()) {
			scene->heightMap()->loadFacture(facture.width(), facture.height(), facture.bits());
		}

		emit updateGL();
	}
}

void MainWindow::showMesh(bool show) {
	if(scene->hasHeightMap()) {
		this->scene->heightMap()->setShowMesh(show);
		emit updateGL();
	}
}
