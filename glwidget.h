#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    int tesselationLevel = 0;
    void drawCube();
    
    float zoom = -3.0;
    float xtrans = 0.0;
    float ytrans = 0.0;
    bool dragging = false;

    QPoint lastPoint;
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);

public slots:
    void setWireframeShading();
    void setFlatShading();
    void setGouraudShading();
    void setTesselation(int tesselationLevel);
};

#endif
