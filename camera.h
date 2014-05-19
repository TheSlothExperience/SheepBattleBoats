#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
protected:
    QVector3D initialPosition;
    QVector3D position;
    QQuaternion initialOrientation;
    QQuaternion orientation;

    QVector3D poi;
    
    QMatrix4x4 cameraMatrix;
    QMatrix4x4 projectionMatrix;

    QQuaternion M4toQuat(QMatrix4x4 mat) {
	double trace = mat(0,0) + mat(1,1) + mat(2,2) + 1;
	double s = 0.5 / sqrt(trace);
	double w = 0.25 / s;
	double x = (mat(2, 1) - mat(1, 2)) * s;
	double y = (mat(0, 2) - mat(2, 0)) * s;
	double z = (mat(1, 0) - mat(0, 1)) * s;
	return QQuaternion(w, x, y, z);
    }

public:
    QMatrix4x4 getCameraMatrix() {
	cameraMatrix.setToIdentity();
	cameraMatrix.rotate(initialOrientation);
	cameraMatrix.translate(-initialPosition);
	cameraMatrix.rotate(orientation);
	cameraMatrix.translate(-position);
	return cameraMatrix;
    };
    QMatrix4x4 getProjectionMatrix() {return this->projectionMatrix;};

    //virtual void translate(QVector3D t) = 0;
    virtual void translate(double x, double y, double z) {
	this->position += QVector3D(x, y, z);
    }

    virtual void rotate(QQuaternion q) {
	this->orientation = q * orientation;
    }
    
    virtual void reset() {
	position = QVector3D();
	orientation = QQuaternion();
	poi = QVector3D();
    }
    virtual void resize(double width, double height) = 0;

};

#endif
