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
    shaders.geometryPassProgram = new QOpenGLShaderProgram();
    shaders.lightPassProgram = new QOpenGLShaderProgram();
	shaders.shaderProgram = new QOpenGLShaderProgram();
	shaders.canvasProgram = new QOpenGLShaderProgram();
	shaders.quadviewProgram = new QOpenGLShaderProgram();
	shaders.storeDepthProgram = new QOpenGLShaderProgram();
	shaders.gaussianBlurVProgram = new QOpenGLShaderProgram();
	shaders.gaussianBlurHProgram = new QOpenGLShaderProgram();
}

GLWidgetContext::~GLWidgetContext()
{
}


void GLWidgetContext::initializeGL()
{
	glEnable(GL_TEXTURE_3D);
    loadShaders(":/shaders/DS_geometryPass.vert", ":/shaders/DS_geometryPass.frag", shaders.shaderProgram);
    //loadShaders(":/shaders/xtoon.vert", ":/shaders/xtoon.frag", shaders.shaderProgram);
    //loadShaders(":/shaders/phong.vert", ":/shaders/phong.frag", shaders.shaderProgram);

	shaders.shaderProgram->bind();

	perspectiveMatLocation = shaders.shaderProgram->uniformLocation("perspectiveMatrix");
	normalMatLocation = shaders.shaderProgram->uniformLocation("normalMatrix");
	modelViewMatLocation = shaders.shaderProgram->uniformLocation("modelViewMatrix");
	lightPositionLocation = shaders.shaderProgram->uniformLocation("lightPosition");


	shaders.shaderProgram->release();

    loadShaders(":/shaders/DS_lightPass.vert", ":/shaders/DS_lightPass.frag", shaders.lightPassProgram);
    loadShaders(":/shaders/identity.vert", ":/shaders/canvas.frag", shaders.canvasProgram);
    loadShaders(":/shaders/viewpoint.vert", ":/shaders/viewpoint.frag", shaders.quadviewProgram);

    loadShaders(":/shaders/storeDepth.vert", ":/shaders/storeDepth.frag", shaders.storeDepthProgram);
    loadShaders(":/shaders/identity.vert", ":/shaders/gaussianBlurV.frag", shaders.gaussianBlurVProgram);
    loadShaders(":/shaders/identity.vert", ":/shaders/gaussianBlurH.frag", shaders.gaussianBlurHProgram);

}


void GLWidgetContext::paintGL()
{
}

void GLWidgetContext::resizeGL(int, int)
{
}

void GLWidgetContext::loadShaders(QString vstring, QString fstring, QOpenGLShaderProgram *prog)
{
	QOpenGLShader *vshader = NULL;
	QOpenGLShader *fshader = NULL;
	loadShaders(vstring, fstring, vshader, fshader, prog);
}

/*
 * Load the vertex shader `vshader` and the fragment shader
 * `fshader`, compiles them and links them to the shader program.
 * Outputs warnings and compilation errors should an error occur.
 */
void GLWidgetContext::loadShaders(QString vstring, QString fstring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShaderProgram *prog)
{
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
			qWarning() << "Fragment shader error" << fshader->log();
		}
	} else {
		qWarning() << "Fragment shader source file " << fstring << " not found.";
	}

	prog->link();
}


void GLWidgetContext::loadShaders(QString vstring, QString fstring, QString tcstring, QString testring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShader *tcshader, QOpenGLShader *teshader, QOpenGLShaderProgram *prog)
{
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
			qWarning() << "Fragment shader error" << fshader->log();
		}
	} else {
		qWarning() << "Fragment shader source file " << fstring << " not found.";
	}

	QFileInfo tcsh(tcstring);
	if (tcsh.exists()) {
		tcshader = new QOpenGLShader(QOpenGLShader::TessellationControl);
		if (tcshader->compileSourceFile(tcstring)) {
			prog->addShader(tcshader);
		} else {
			qWarning() << "Tessellation control shader error" << tcshader->log();
		}
	} else {
		qWarning() << "Tesselation control source file " << tcstring << " not found.";
	}

	QFileInfo tesh(testring);
	if (tesh.exists()) {
		teshader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation);
		if (teshader->compileSourceFile(testring)) {
			prog->addShader(teshader);
		} else {
			qWarning() << "Tessellation evaluation shader error" << teshader->log();
		}
	} else {
		qWarning() << "Tesselation evaluation source file " << testring << " not found.";
	}

	prog->link();
}

void GLWidgetContext::loadShaders(QString vstring, QString fstring, QString tcstring, QString testring, QString gstring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShader *tcshader, QOpenGLShader *teshader, QOpenGLShader *gshader, QOpenGLShaderProgram *prog)
{
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
			qWarning() << "Fragment shader error" << fshader->log();
		}
	} else {
		qWarning() << "Fragment shader source file " << fstring << " not found.";
	}

	QFileInfo tcsh(tcstring);
	if (tcsh.exists()) {
		tcshader = new QOpenGLShader(QOpenGLShader::TessellationControl);
		if (tcshader->compileSourceFile(tcstring)) {
			prog->addShader(tcshader);
		} else {
			qWarning() << "Tessellation control shader error" << tcshader->log();
		}
	} else {
		qWarning() << "Tesselation control source file " << tcstring << " not found.";
	}

	QFileInfo tesh(testring);
	if (tesh.exists()) {
		teshader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation);
		if (teshader->compileSourceFile(testring)) {
			prog->addShader(teshader);
		} else {
			qWarning() << "Tessellation evaluation shader error" << teshader->log();
		}
	} else {
		qWarning() << "Tesselation evaluation source file " << testring << " not found.";
	}

	QFileInfo gsh(gstring);
	if (gsh.exists()) {
		gshader = new QOpenGLShader(QOpenGLShader::Geometry);
		if (gshader->compileSourceFile(gstring)) {
			prog->addShader(gshader);
		} else {
			qWarning() << "Geometry shader error" << gshader->log();
		}
	} else {
		qWarning() << "Geometry evaluation source file " << gstring << " not found.";
	}

	prog->link();
}
