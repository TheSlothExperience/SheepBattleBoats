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
    glShadeModel(GL_FLAT);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0, 0, 0, 0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    loadShaders(":/phong.vert", ":/phong.frag");
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(xtrans, ytrans, zoom);
    glMultMatrixf(this->cubeRotationMatrix.constData());
    drawCube();
    glPopMatrix();
    
}

void GLWidget::resizeGL(int width, int height)
{   
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
    shaderProgram->release();
    glPolygonMode(GL_FRONT, GL_LINE);
    glShadeModel(GL_FLAT);
    updateGL();
}

void GLWidget::setFlatShading() {
    shaderProgram->release();
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_FLAT);
    updateGL();
}

void GLWidget::setGouraudShading() {
    shaderProgram->release();
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_SMOOTH);
    updateGL();
}

void GLWidget::setPhongShading() {
    shaderProgram->bind();
    updateGL();
}

void GLWidget::setTesselation(int tesselationLevel) {
    this->tesselationLevel = tesselationLevel;
    updateGL();
}

void GLWidget::loadShaders(QString vshader, QString fshader) {
    shaderProgram = new QOpenGLShaderProgram;
    
    QFileInfo vsh(vshader);
    if (vsh.exists()) {
	this->vshader = new QOpenGLShader(QOpenGLShader::Vertex);
	if (this->vshader->compileSourceFile(vshader)) {
	    shaderProgram->addShader(this->vshader);
	} else {
	    qWarning() << "Vertex shader error" << this->vshader->log();
	}
    } else {
	qWarning() << "Vertex shader source file " << vshader << " not found.";
    }
    
    QFileInfo fsh(fshader);
    if (fsh.exists()) {
	this->fshader = new QOpenGLShader(QOpenGLShader::Fragment);
	if (this->fshader->compileSourceFile(fshader)) {
	    shaderProgram->addShader(this->fshader);
	} else {
	    qWarning() << "Vertex shader error" << this->fshader->log();
	}
    } else {
	qWarning() << "Vertex shader source file " << fshader << " not found.";
    }

    shaderProgram->link();
}

void GLWidget::resetCamera() {
    this->zoom = 0.0;
    this->xtrans = 0.0;
    this->ytrans = 0.0;
    this->cubeRotationMatrix.setToIdentity();
    updateGL();
}

void GLWidget::drawCube() {
    int squares = pow(2, tesselationLevel);
    double step = 1.0/squares;
    GLfloat material_Ks[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
    //front
    glColor3f(0.0,0.0,1.0);
    glNormal3f(0.0, 0.0, 1.0);
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
    glNormal3f(1.0, 0.0, 0.0);
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
    glNormal3f(0.0, 1.0, 0.0);
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
    glNormal3f(0.0, 0.0, -1.0);
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
    glNormal3f(-1.0, 0.0, 0.0);
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
    glNormal3f(0.0, -1.0, 0.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_TRIANGLE_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + y*step);
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + (y+1)*step);
	}
	glEnd();
    }
}
