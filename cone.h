#ifndef CONE_H
#define CONE_H

#include <GL/gl.h>

#include "primitive.h"

class Cone : public Primitive
{
private:
    GLuint indexBufferObject;
    int count;
public:
    Cone(int tesselationLevel);
    ~Cone();
    void draw();
};

#endif
