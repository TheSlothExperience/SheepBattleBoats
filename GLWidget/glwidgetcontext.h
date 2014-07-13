#ifndef GLWIDGETCONTEXT_H
#define GLWIDGETCONTEXT_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShader>

#include <stack>

#include "scene.h"
#include "cube.h"
#include "primitive.h"

class GLWidgetContext : public QGLWidget
{
	Q_OBJECT

	public:
	GLWidgetContext(QWidget *parent = 0);
	~GLWidgetContext();
	GLuint getPerspectiveMatLocation() {return this->perspectiveMatLocation;};
	GLuint getModelViewMatLocation() {return this->modelViewMatLocation;};
	GLuint getNormalMatLocation() {return this->normalMatLocation;};
	GLuint getLightPositionLocation() {return this->lightPositionLocation;};
	QOpenGLShaderProgram* getShaderProgram() {return this->shaderProgram;};
	QOpenGLShaderProgram* getCanvasProgram() {return this->canvasProgram;};
	QOpenGLShaderProgram* getQuadViewProgram() {return this->quadviewProgram;};

	void initializeGL();

	private:
	QOpenGLShaderProgram *shaderProgram;
	QOpenGLShaderProgram *canvasProgram;
	QOpenGLShaderProgram *quadviewProgram;
    QOpenGLShaderProgram *comicborderProgram;

	QOpenGLShader *vphong;
	QOpenGLShader *fphong;

	QOpenGLShader *vcanvas;
	QOpenGLShader *fcanvas;

	QOpenGLShader *vquadview;
    QOpenGLShader *fquadview;

    QOpenGLShader *vcomicborder;
    QOpenGLShader *fcomicborder;

	void loadShaders(QString vstring, QString fstring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShaderProgram *prog);
	void loadShaders(QString vstring, QString fstring, QString tcstring, QString testring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShader *tcshader, QOpenGLShader *teshader, QOpenGLShaderProgram *prog);
	void loadShaders(QString vstring, QString fstring, QString tcstring, QString testring, QString gstring, QOpenGLShader *vshader, QOpenGLShader *fshader, QOpenGLShader *tcshader, QOpenGLShader *teshader, QOpenGLShader *gshader, QOpenGLShaderProgram *prog);

	GLuint perspectiveMatLocation;
	GLuint modelViewMatLocation;
	GLuint normalMatLocation;
	GLuint lightPositionLocation;

	protected:
	void paintGL();
	void resizeGL(int width, int height);
};

#endif
