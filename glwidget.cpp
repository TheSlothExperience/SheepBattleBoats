#define GL_GLEXT_PROTOTYPES

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include "GL/glu.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      tesselationLevel(0),
      zoom(0.0), xtrans(0.0), ytrans(0.0), dragging(false)
{
    modelViewMatrixStack.push(QMatrix4x4());
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    loadShaders(":/phong.vert", ":/phong.frag");

    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    static const GLfloat g_vertex_buffer_data[] = {
     	-0.5f,-0.5f,-0.5f, // triangle 1 : begin
     	-0.5f,-0.5f, 0.5f,
     	-0.5f, 0.5f, 0.5f, // triangle 1 : end
     	0.5f, 0.5f,-0.5f, // triangle 2 : begin
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f,-0.5f, // triangle 2 : end
     	0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	-0.5f,-0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f, 0.5f,-0.5f,
     	-0.5f, 0.5f,-0.5f,
     	0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f
    };
    
    static const GLfloat g_normal_buffer_data[] = {
     	-1.0f,-0.0f,-0.0f, // left
     	-1.0f,-0.0f, 0.0f,
     	-1.0f, 0.0f, 0.0f, 
     	0.0f, 0.0f,-1.0f, // back
     	-0.0f,-0.0f,-1.0f,
     	-0.0f, 0.0f,-1.0f,
     	0.0f,-1.0f, 0.0f, // bottom
     	-0.0f,-1.0f,-0.0f,
     	0.0f,-1.0f,-0.0f,
     	0.0f, 0.0f,-1.0f, // back
     	0.0f,-0.0f,-1.0f,
     	-0.0f,-0.0f,-1.0f,
     	-1.0f,-0.0f,-0.0f,// left
     	-1.0f, 0.0f, 0.0f,
     	-1.0f, 0.0f,-0.0f,
     	0.0f,-1.0f, 0.0f, // bottom
     	-0.0f,-1.0f, 0.0f,
     	-0.0f,-1.0f,-0.0f,
     	-0.0f, 0.0f, 1.0f, // front 
     	-0.0f,-0.0f, 1.0f,
     	0.0f,-0.0f, 1.0f,
     	1.0f, 0.0f, 0.0f,  //right
     	1.0f,-0.0f,-0.0f,
     	1.0f, 0.0f,-0.0f,
     	1.0f,-0.0f,-0.0f, // right
     	1.0f, 0.0f, 0.0f,
     	1.0f,-0.0f, 0.0f,
     	0.0f, 1.0f, 0.0f, // top
     	0.0f, 1.0f,-0.0f,
     	-0.0f, 1.0f,-0.0f,
     	0.0f, 1.0f, 0.0f, // top
     	-0.0f, 1.0f,-0.0f,
     	-0.0f, 1.0f, 0.0f,
     	0.0f, 0.0f, 1.0f, // front
     	-0.0f, 0.0f, 1.0f,
     	0.0f, 0.0f, 1.0f
    };	
    shaderProgram->bind();
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //set perspective matrix
    perspectiveMatLocation = shaderProgram->uniformLocation("perspectiveMatrix");
    QMatrix4x4 perspectiveMatrix;
    perspectiveMatrix.perspective(45, (float)this->width() / (float)this->height(), 0.1, 100);
    glUniformMatrix4fv(perspectiveMatLocation, 1, GL_FALSE, perspectiveMatrix.constData());

    cameraMatrix.lookAt(QVector3D(0.0, 0.0, 3.0), QVector3D(), QVector3D(0.0, 1.0, 0.0));
    modelViewMatrixStack.top() *= cameraMatrix;   
    modelMatLocation = shaderProgram->uniformLocation("modelViewMatrix");
    normalMatLocation = shaderProgram->uniformLocation("normalMatrix");

    //Create point light source and transform into Eye Coords
    lightPosition = QVector4D(0.5, 0.0, 2.0, 1.0);
    QVector4D lightDir = cameraMatrix * lightPosition;
    GLfloat lightDirArray[3] = {lightDir.x(), lightDir.y(), lightDir.z()};
    lightPositionLocation = shaderProgram->uniformLocation("lightPosition");
    glUniform3fv(lightPositionLocation, 1, lightDirArray);
    shaderProgram->release();
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->bind();
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    
    modelViewMatrixStack.top().translate(xtrans, ytrans, zoom);
    
    //Convert the quat to a matrix, may be a performance leak.
    QMatrix4x4 tempRot;
    tempRot.rotate(this->cubeRotationQuat.normalized());
    modelViewMatrixStack.top() *= tempRot;
    glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, modelViewMatrixStack.top().constData());

    glUniformMatrix4fv(normalMatLocation, 1, GL_FALSE, modelViewMatrixStack.top().inverted().transposed().constData());
    
    //drawCube();
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3*12);
    
    //glPopMatrix();
    modelViewMatrixStack.pop();
    
}

void GLWidget::resizeGL(int width, int height)
{
    //Set up the viewport and perspective projection with a FOV of 45
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

/*
 * Load the vertex shader `vshader` and the fragment shader
 * `fshader`, compiles them and links them to the shader program.
 * Outputs warnings and compilation errors should an error occur.
 */
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
    this->cubeRotationQuat = QQuaternion();
    updateGL();
}

/*
 * Draw an unit cube centered on (0,0).
 */
void GLWidget::drawCube() {
    //Only use powers of two to assure a nice cube
    int squares = pow(2, tesselationLevel);
    double step = 1.0/squares;

    //Set the specular properties of the vertices.
    GLfloat material_Ks[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0);
    
    //front
    glColor3f(0.0,0.0,1.0);
    glNormal3f(0.0, 0.0, 1.0);
    for (int y = 0; y < squares; y++) {
	glBegin(GL_QUAD_STRIP);
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
	glBegin(GL_QUAD_STRIP);
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
	glBegin(GL_QUAD_STRIP);
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
	glBegin(GL_QUAD_STRIP);
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
	glBegin(GL_QUAD_STRIP);
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
	glBegin(GL_QUAD_STRIP);
	for (int x = 0; x <= squares; x++) {
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + y*step);
	    glVertex3f(0.5 - x*step, -0.5, -0.5 + (y+1)*step);
	}
	glEnd();
    }
}
