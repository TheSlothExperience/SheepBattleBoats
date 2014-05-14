#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShader>

#include <stack>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    int tesselationLevel;
    void drawCube();

    QQuaternion cubeRotationQuat;
    float zoom;
    float xtrans;
    float ytrans;
    bool dragging;

    QPoint lastPoint;

    QOpenGLShaderProgram *shaderProgram;
    QOpenGLShader *vshader;
    QOpenGLShader *fshader;
    void loadShaders(QString vshader, QString fshader);

    GLuint vao;
    GLuint vertexBufferObject;
    GLuint normalBufferObject;

    int positionLocation;
    GLuint perspectiveMatLocation;
    GLuint modelMatLocation;

    QMatrix4x4 cameraMatrix;
    std::stack<QMatrix4x4> modelViewMatrixStack;
    
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
