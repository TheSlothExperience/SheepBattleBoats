#include "orthocamera.h"
#include "QVector3D"

OrthoCamera::OrthoCamera(double x, double y, double z, double width, double height) {
    this->initialPosition = QVector3D(x, y, z);
    QVector3D up = QVector3D::crossProduct(QVector3D(-z, 0, x), initialPosition);
    this->cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    this->projectionMatrix.ortho(-width/height, width/height, -1.0f, 1.0f, 0.0f, 100.0f);
    this->poi = initialPosition.length();
}

OrthoCamera::OrthoCamera(QVector3D position, double width, double height) {
    this->initialPosition = QVector3D(position);
    QVector3D up = QVector3D::crossProduct(initialPosition, QVector3D(-position.z(), 0, position.x()));
    this->cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    this->projectionMatrix.ortho(0, width, 0, height, 0.1, 100);
    this->poi = initialPosition.length();
}
