#include "perspectivecamera.h"
#include "QVector3D"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(double x, double y, double z, double width, double height) {
    this->initialPosition = QVector3D(x, y, z);
    QVector3D up;
    if(x != 0 || z != 0) {
	up = QVector3D::crossProduct(QVector3D(-z, 0, x), initialPosition);
    } else {
	//Looking from the top
	up = QVector3D(0.0, 0.0, -1.0);
    }
    cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    initialOrientation = M4toQuat(cameraMatrix);
    this->width = width;
    this->height = height;
    this->fov = 45;
    this->projectionMatrix.perspective(fov, width / height, 0.1, 100); 
    poi = QVector3D(0,0,0);
}

PerspectiveCamera::PerspectiveCamera(QVector3D position, double width, double height) {
    PerspectiveCamera(position.x(), position.y(), position.z(), width, height);
}

void PerspectiveCamera::resize(double width, double height) {
    this->width = width;
    this->height = height;
}

QMatrix4x4 PerspectiveCamera::getProjectionMatrix() {
    this->projectionMatrix.setToIdentity();
    this->projectionMatrix.perspective(fov, width / height, 0.1, 100);
    return projectionMatrix;
}

void PerspectiveCamera::reset() {
    position = QVector3D();
    orientation = QQuaternion();
    poi = QVector3D();
    fov = 45;
}

void PerspectiveCamera::zoom(double z) {
    this->fov -= z * 3;
}
