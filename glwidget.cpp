#define GL_GLEXT_PROTOTYPES

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include "GL/glu.h"
#include <math.h>
#include <iostream>

#include "perspectivecamera.h"
#include "orthocamera.h"

GLWidget::GLWidget(QWidget *parent, const QGLWidget *shareWidget)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shareWidget),
      tesselationLevel(0),
      zoom(0.0), xtrans(0.0), ytrans(0.0), dragging(false)
{
    scene = NULL;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    //Set up OpenGL incantations
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Set up a spot light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->bind();
    
    glUniformMatrix4fv(perspectiveMatLocation, 1, GL_FALSE, camera->getProjectionMatrix().constData());
    
    if(scene != NULL) {
	scene->draw(camera->getCameraMatrix());	
    } else {
	std::cout << "no scene yet" << std::endl;
    }
    
    shaderProgram->release();
    
}

void GLWidget::resizeGL(int width, int height)
{
    this->camera->resize(width, height);
    glViewport(0, 0, width, height);
}

void GLWidget::setProjectionLocation(GLuint pL) {
    this->perspectiveMatLocation = pL;
}
void GLWidget::setCamera(Camera *camera) {
    this->camera = camera;
}

void GLWidget::setPerspectiveCamera(double x, double y, double z) {
    this->camera = new PerspectiveCamera(x, y, z, this->width(), this->height());
}

void GLWidget::setOrthoCamera(double x, double y, double z) {
    this->camera = new OrthoCamera(x, y, z, this->width(), this->height());
}

void GLWidget::setScene(Scene *scene) {
    this->scene = scene;
}

void GLWidget::setShaderProgram(QOpenGLShaderProgram *sp) {
    this->shaderProgram = sp;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton) {
	lastPoint = event->pos();
	dragging = true;
    }
}

/*
 * Project coordinates to the surface of the sphere or to
 * the hyperbolic surface, if it doesn's lie on the sphere.
 */
double z(double x, double  y) {
    double length = sqrt(x*x + y*y);
    //Let the radius of the sphere be 1
    if (length <= 1.0/2.0) {
	return sqrt(1.0 - length);
    } else {
	return (1.0/2.0) / sqrt(length);
    }
    
}

double clampUnit(double x) {
    return std::min(1.0, std::max(-1.0, x));
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::RightButton) && dragging) {
	this->xtrans += (event->pos().x() - lastPoint.x()) / 100.0;
	this->ytrans -= (event->pos().y() - lastPoint.y()) / 100.0; //Qt y-coord is inverted
	this->lastPoint = event->pos();
	updateGL();
    }
    
    if ((event->buttons() & Qt::LeftButton) && dragging) {
	//Here we implement the trackball. Sample two points on the sphere and
	//calculate their angle to use as the rotation.
	
	//normalize to intervals [-1,1]
	double lastx = clampUnit(lastPoint.x() / (this->size().width() / 2.0) - 1.0);
	double lasty = clampUnit(-(lastPoint.y() / (this->size().height() / 2.0) - 1.0));
	
	double newx = clampUnit(event->pos().x() / (this->size().width() / 2.0) - 1.0);
	double newy = clampUnit(-(event->pos().y() / (this->size().height() / 2.0) - 1.0));

	//Project the two points into the sphere (or the hyperbolic plane)
	QVector3D v1(lastx, lasty, z(lastx, lasty));
	v1.normalize();
	QVector3D v2(newx, newy, z(newx, newy));
	v2.normalize();
	
	//Determine the normal of the generated plane through the center of the sphere
	QVector3D normal = QVector3D::crossProduct(v1, v2);
	double theta = acos(QVector3D::dotProduct(v1, v2)) / 3.0;
	
	//angle/2.0, because the quats double cover SO(3)
	QQuaternion newRot(cos(theta/2.0), sin(theta/2.0) * normal.normalized());
	
	//Pre-multiply to get the rot in local coords
	this->cubeRotationQuat = newRot.normalized() * this->cubeRotationQuat;
	updateGL();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton) {
	dragging = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    this->zoom += event->delta()/300.0;
    updateGL();
}


void GLWidget::setTesselation(int tesselationLevel) {
    this->tesselationLevel = tesselationLevel;
    updateGL();
}

void GLWidget::resetCamera() {
    this->zoom = 0.0;
    this->xtrans = 0.0;
    this->ytrans = 0.0;
    this->cubeRotationQuat = QQuaternion();
    updateGL();
}
