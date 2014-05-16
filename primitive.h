#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/gl.h>

class Primitive
{
protected:
    GLuint vertexBufferObject;
    GLuint normalBufferObject;

public:
    virtual void draw() = 0;
};

#endif //PRIMITIVE_H
