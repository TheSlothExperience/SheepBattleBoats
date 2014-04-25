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
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    //glPolygonMode(GL_FRONT, GL_LINE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    
    static GLfloat lightPosition[4] = {0.5, 0.0, 2.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void drawCube(int tesselation = 0) {
    int squares = pow(2, tesselation);
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

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(45, 0.0, 1.0, 0.0);
    glRotatef(-20, 1.0, 0.0, 0.0);
    drawCube(3);
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
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
}
