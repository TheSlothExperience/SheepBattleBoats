#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "camera.h"

class OrthoCamera : public Camera
{
public:
    OrthoCamera(double x, double y, double z, double width, double height);
    OrthoCamera(QVector3D position, double width, double height);
};

#endif
