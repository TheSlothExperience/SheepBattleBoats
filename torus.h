#ifndef TORUS_H
#define TORUS_H

#include <GL/gl.h>

#include "primitive.h"

class Torus : public Primitive
{
private:
    GLuint indexBufferObject;
    int count;
    int numVerts;
    int parts;
    int idxPerPart;
public:
    Torus(int tesselationLevel);
    ~Torus();
    void draw();
};

#endif
