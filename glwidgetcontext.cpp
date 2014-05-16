#define GL_GLEXT_PROTOTYPES

#include "glwidgetcontext.h"


#include <QtGui>
#include <GL/gl.h>
#include "GL/glu.h"
#include <math.h>
#include <math.h>
#include <iostream>

GLWidgetContext::GLWidgetContext(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
}

GLWidgetContext::~GLWidgetContext()
{
}


void GLWidgetContext::initializeGL()
{   
    loadShaders(":/phong.vert", ":/phong.frag");

    
    shaderProgram->bind();

    perspectiveMatLocation = shaderProgram->uniformLocation("perspectiveMatrix");
    normalMatLocation = shaderProgram->uniformLocation("normalMatrix");
    modelViewMatLocation = shaderProgram->uniformLocation("modelViewMatrix");
    lightPositionLocation = shaderProgram->uniformLocation("lightPosition");

    
    shaderProgram->release();
}


void GLWidgetContext::paintGL()
{
}

void GLWidgetContext::resizeGL(int width, int height)
{
}

/*
 * Load the vertex shader `vshader` and the fragment shader
 * `fshader`, compiles them and links them to the shader program.
 * Outputs warnings and compilation errors should an error occur.
 */
void GLWidgetContext::loadShaders(QString vshader, QString fshader) {
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
