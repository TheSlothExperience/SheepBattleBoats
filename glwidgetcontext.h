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
    GLWidgetContext(QWidget *parent = 0, const QGLWidget *shareWidget = 0);
    ~GLWidgetContext();
    GLuint getPerspectiveMatLocation() {return this->perspectiveMatLocation;};
    GLuint getModelViewMatLocation() {return this->modelViewMatLocation;};
    GLuint getNormalMatLocation() {return this->normalMatLocation;};
    GLuint getLightPositionLocation() {return this->lightPositionLocation;};
    QOpenGLShaderProgram* getShaderProgram() {return this->shaderProgram;};
    
    void initializeGL();

private:
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLShader *vshader;
    QOpenGLShader *fshader;
    void loadShaders(QString vshader, QString fshader);

    GLuint perspectiveMatLocation;
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;
    
protected:
    void paintGL();
    void resizeGL(int width, int height);
    // void mousePressEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);
    // void wheelEvent (QWheelEvent *event);
};

#endif
