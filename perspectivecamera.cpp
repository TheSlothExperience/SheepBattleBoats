#include "perspectivecamera.h"
#include "QVector3D"

PerspectiveCamera::PerspectiveCamera(double x, double y, double z, double width, double height) {
    this->initialPosition = QVector3D(x, y, z);
    QVector3D up;
    if(x != 0 || z != 0) {
	up = QVector3D::crossProduct(QVector3D(-z, 0, x), initialPosition);
    } else {
	//Looking from the top
	up = QVector3D(0.0, 0.0, -1.0);
    }
    this->cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    this->projectionMatrix.perspective(45, width / height, 0.1, 100);
    this->poi = initialPosition.length();
}

PerspectiveCamera::PerspectiveCamera(QVector3D position, double width, double height) {
    this->initialPosition = QVector3D(position);
    QVector3D up = QVector3D::crossProduct(initialPosition, QVector3D(-position.z(), 0, position.x()));
    this->cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    this->projectionMatrix.perspective(45, width / height, 0.1, 100);
    this->poi = initialPosition.length();
}

void PerspectiveCamera::resize(double width, double height) {
    this->projectionMatrix.setToIdentity();
    this->projectionMatrix.perspective(45, width / height, 0.1, 100);
}
