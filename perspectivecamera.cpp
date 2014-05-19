#include "perspectivecamera.h"
#include "QVector3D"

PerspectiveCamera::PerspectiveCamera(double x, double y, double z, double width, double height) {
    this->initialPosition = QVector3D(x, y, z);
    //this->position = initialPosition;
    QVector3D up;
    if(x != 0 || z != 0) {
	up = QVector3D::crossProduct(QVector3D(-z, 0, x), initialPosition);
    } else {
	//Looking from the top
	up = QVector3D(0.0, 0.0, -1.0);
    }
    cameraMatrix.lookAt(initialPosition, QVector3D(0, 0, 0), up);
    initialOrientation = M4toQuat(cameraMatrix);
    //orientation = initialOrientation;
    this->projectionMatrix.perspective(45, width / height, 0.1, 100);
    poi = QVector3D(0,0,0);
}

PerspectiveCamera::PerspectiveCamera(QVector3D position, double width, double height) {
    PerspectiveCamera(position.x(), position.y(), position.z(), width, height);
}

void PerspectiveCamera::resize(double width, double height) {
    this->projectionMatrix.setToIdentity();
    this->projectionMatrix.perspective(45, width / height, 0.1, 100);
}
