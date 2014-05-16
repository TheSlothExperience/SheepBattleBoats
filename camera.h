#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
protected:
    QVector3D initialPosition;
    QMatrix4x4 cameraMatrix;
    QMatrix4x4 projectionMatrix;
    double poi;

public:
    QMatrix4x4 getCameraMatrix() {return this->cameraMatrix;};
    QMatrix4x4 getProjectionMatrix() {return this->projectionMatrix;};
};

#endif
