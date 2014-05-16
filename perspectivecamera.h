#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(double x, double y, double z, double width, double height);
    PerspectiveCamera(QVector3D position, double width, double height);
};

#endif
