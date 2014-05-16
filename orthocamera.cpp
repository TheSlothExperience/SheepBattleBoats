#include "orthocamera.h"
#include "QVector3D"
#include <iostream>

OrthoCamera::OrthoCamera(double x, double y, double z, double width, double height) {
    this->initialPosition = QVector3D(x, y, z);
    QVector3D up;
    if(x != 0 || z != 0) {
	up = QVector3D::crossProduct(QVector3D(-z, 0, x), initialPosition);
    } else {
	//Looking from the top
	up = QVector3D(0.0, 0.0, -1.0);
    }
    this->cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    this->projectionMatrix.ortho(-width/height, width/height, -1.0f, 1.0f, -100.0f, 100.0f);
    this->poi = initialPosition.length();
}

OrthoCamera::OrthoCamera(QVector3D position, double width, double height) {
    OrthoCamera(position.x(), position.y(), position.z(), width, height);
}

void OrthoCamera::resize(double width, double height) {
    this->projectionMatrix.setToIdentity();
    this->projectionMatrix.ortho(-width/height, width/height, -1.0f, 1.0f, -100.0f, 100.0f);
}
