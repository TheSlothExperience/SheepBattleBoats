#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShader>

#include <stack>

#include "scene.h"
#include "cube.h"
#include "primitive.h"
#include "camera.h"

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
    void setCamera(Camera *camera);
    void setPerspectiveCamera(double x, double y, double z);
    void setOrthoCamera(double x, double y, double z);
    void setProjectionLocation(GLuint pL);
    void initializeGL();

private:
    int tesselationLevel;

    QQuaternion cubeRotationQuat;
    float zoom;
    float xtrans;
    float ytrans;
    bool dragging;
    
    QPoint lastPoint;

    QOpenGLShaderProgram *shaderProgram;

    GLuint perspectiveMatLocation;
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;

    Camera *camera;

    Scene *scene;
    
protected:
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
