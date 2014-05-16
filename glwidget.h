#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShader>

#include <stack>

#include "scene.h"
#include "cube.h"
#include "primitive.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setScene(Scene *scene);
    void setShaderProgram(QOpenGLShaderProgram *sp);

private:
    int tesselationLevel;
    void drawCube();

    QQuaternion cubeRotationQuat;
    float zoom;
    float xtrans;
    float ytrans;
    bool dragging;

    QVector4D lightPosition;
    
    QPoint lastPoint;

    QOpenGLShaderProgram *shaderProgram;
    QOpenGLShader *vshader;
    QOpenGLShader *fshader;
    void loadShaders(QString vshader, QString fshader);

    GLuint perspectiveMatLocation;
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;

    QMatrix4x4 cameraMatrix;

    Scene *scene;
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);

public slots:
    void setTesselation(int tesselationLevel);
    void resetCamera();
};

#endif
