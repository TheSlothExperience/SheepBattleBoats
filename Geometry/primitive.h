#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/gl.h>

class Primitive
{
protected:
    GLuint vertexBufferObject;
    GLuint normalBufferObject;

public:
    virtual ~Primitive() = 0;
    virtual void draw() = 0;
};

inline Primitive::~Primitive() {}
#endif //PRIMITIVE_H
