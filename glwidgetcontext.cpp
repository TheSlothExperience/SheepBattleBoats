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
    shaderProgram = new QOpenGLShaderProgram();
    textureProgram = new QOpenGLShaderProgram();
}

GLWidgetContext::~GLWidgetContext()
{
}


void GLWidgetContext::initializeGL()
{   
    loadShaders(":/phong.vert", ":/phong.frag", vphong, fphong, shaderProgram);

    shaderProgram->bind();

    perspectiveMatLocation = shaderProgram->uniformLocation("perspectiveMatrix");
    normalMatLocation = shaderProgram->uniformLocation("normalMatrix");
    modelViewMatLocation = shaderProgram->uniformLocation("modelViewMatrix");
    lightPositionLocation = shaderProgram->uniformLocation("lightPosition");

    
    shaderProgram->release();
    
    loadShaders(":/identity.vert", ":/canvas.frag", vtexture, ftexture, textureProgram);
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
void GLWidgetContext::loadShaders(QString vstring, QString fstring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShaderProgram *prog) {
    QFileInfo vsh(vstring);
    if (vsh.exists()) {
	vshader = new QOpenGLShader(QOpenGLShader::Vertex);
	if (vshader->compileSourceFile(vstring)) {
	    prog->addShader(vshader);
	} else {
	    qWarning() << "Vertex shader error" << vshader->log();
	}
    } else {
	qWarning() << "Vertex shader source file " << vstring << " not found.";
    }
    
    QFileInfo fsh(fstring);
    if (fsh.exists()) {
	fshader = new QOpenGLShader(QOpenGLShader::Fragment);
	if (fshader->compileSourceFile(fstring)) {
	    prog->addShader(fshader);
	} else {
	    qWarning() << "Vertex shader error" << fshader->log();
	}
    } else {
	qWarning() << "Vertex shader source file " << fstring << " not found.";
    }

    prog->link();
}
