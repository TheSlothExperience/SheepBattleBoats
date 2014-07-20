#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/gl.h>
#include<boundingbox.h>

class Primitive
{
protected:
    GLuint vertexBufferObject;
    GLuint normalBufferObject;
    QVector3D shootingDir;
    BoundingBox *bb;

public:
    virtual ~Primitive() = 0;
    virtual void draw() = 0;
    virtual void exeObjBehaviour() {}
    virtual BoundingBox* getBB() {return this->bb;}
};

inline Primitive::~Primitive() {}
#endif //PRIMITIVE_H
