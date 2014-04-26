#include "glwidget.h"

#include <QtGui>
#include <QtOpenGL>
#include "GL/glu.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(xtrans, ytrans, zoom);
    glMultMatrixf(this->cubeRotationMatrix.constData());
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    drawCube();
    glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{   
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton) {
	lastPoint = event->pos();
	dragging = true;
    }
}

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
	//normalize to interval [-1,1]
	double lastx = clampUnit(lastPoint.x() / (this->size().width() / 2.0) - 1.0);
	double lasty = clampUnit(-(lastPoint.y() / (this->size().height() / 2.0) - 1.0));
	
	double newx = clampUnit(event->pos().x() / (this->size().width() / 2.0) - 1.0);
	double newy = clampUnit(-(event->pos().y() / (this->size().height() / 2.0) - 1.0));

	QVector3D v1(lastx, lasty, z(lastx, lasty));
	v1.normalize();
	QVector3D v2(newx, newy, z(newx, newy));
	v2.normalize();
	QVector3D normal = QVector3D::crossProduct(v1, v2);
	double theta = acos(QVector3D::dotProduct(v1, v2)) / 3.0;
	QQuaternion newRot(cos(theta/2.0), sin(theta/2.0) * normal.normalized());
	QMatrix4x4 temp;
	temp.rotate(newRot.normalized());
	this->cubeRotationMatrix = temp * this->cubeRotationMatrix;
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

void GLWidget::setWireframeShading() {
    glPolygonMode(GL_FRONT, GL_LINE);
    glShadeModel(GL_FLAT);
    updateGL();
}

void GLWidget::setFlatShading() {
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_FLAT);
    updateGL();
}

void GLWidget::setGouraudShading() {
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_SMOOTH);
    updateGL();
}

void GLWidget::setTesselation(int tesselationLevel) {
    this->tesselationLevel = tesselationLevel;
    updateGL();
}

void GLWidget::drawCube() {
    int squares = pow(2, tesselationLevel);
    double step = 1.0/squares;

    //front
    glColor3f(0.0,0.0,1.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(-0.5 + x*step, 0.5 - y*step, 0.5);
	    glVertex3f(-0.5 + x*step, 0.5 - (y+1)*step, 0.5);
	}
	glEnd();
    }

    //right
    glColor3f(1.0, 0.0, 0.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(0.5, 0.5 - y*step, 0.5 - x*step);
	    glVertex3f(0.5, 0.5 - (y+1)*step, 0.5 - x*step);
	}
	glEnd();
    }

    //top
    glColor3f(0.0, 1.0, 0.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(-0.5 + x*step, 0.5, -0.5 + y*step);
	    glVertex3f(-0.5 + x*step, 0.5, -0.5 + (y+1)*step);
	}
	glEnd();
    }

    //back
    glColor3f(1.0, 1.0, 0.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(0.5 - x*step, 0.5 - y*step, -0.5);
	    glVertex3f(0.5 - x*step, 0.5 - (y+1)*step, -0.5);
	}
	glEnd();
    }

    //left
    glColor3f(0.0, 1.0, 1.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(-0.5, 0.5 - y*step, -0.5 + x*step);
	    glVertex3f(-0.5, 0.5 - (y+1)*step, -0.5 + x*step);
	}
	glEnd();
    }

    //bottom
    glColor3f(1.0, 0.0, 1.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + y*step);
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + (y+1)*step);
	}
	glEnd();
    }
}
