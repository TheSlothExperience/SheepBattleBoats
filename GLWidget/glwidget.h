#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShader>

#include <stack>
#include <QModelIndex>

#include "scene.h"
#include "cube.h"
#include "primitive.h"
#include "camera.h"
#include <gbuffer.h>
#include "glwidgetcontext.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setScene(Scene *scene);

	void setShaders(Shaders s) {this->shaders = s;}

    void setCamera(Camera *camera);
    void setPerspectiveCamera(double x, double y, double z);
    void setOrthoCamera(double x, double y, double z);
    void setProjectionLocation(GLuint pL);

	QVector3D getCameraWorldPosition() {return camera->getWorldPosition();}
    void initializeGL();

    void setActive(bool active = true);
    bool getActive() {return isActive;}
    void setCameraActive(bool active = true) {this->cameraActive = active;}

	void translateCamera(double x, double y, double z);
	void rotateCamera(float angle);

private:
    int tesselationLevel;

    float zoom;
    bool dragging;
    bool isActive = false;
    bool cameraActive = false;
    int activeID = -1;

    QPoint lastPoint;

	Shaders shaders;

    void renderScene();
    void shadowMapsPass();
	void passShadowMaps(QOpenGLShaderProgram *shader, int texOffset);
    void paintSceneToCanvas();

    GLuint fbo;
    GLuint renderTex;
    GLuint pickingTex;
    GLuint depthBuffer;

    //Diferred Shading
    GLuint colorTexture;
    GLuint normalTexture;
    GLuint textureCoordTexture;


    GLuint canvasQuad;
    GLuint activeLocation;
    GLuint activeColorLocation;

    GLuint perspectiveMatLocation;
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;

    GBuffer gbuffer;

    Camera *camera;

    Scene *scene;

    void DSGeometryPass();
    void DSLightPass();
    void RenderFPS();

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
    void forceGLupdate();
    void changeActiveId(int id);

signals:
    void changedActiveId(int id);
    void changedCurrent(QModelIndex q);
    void translate(double x, double y, double z);
    void rotate(QQuaternion *q);
    void switchActive(GLWidget *active);
};

#endif
