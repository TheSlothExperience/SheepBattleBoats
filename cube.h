#ifndef CUBE_H
#define CUBE_H

#include <GL/gl.h>

#include "primitive.h"

class Cube : public Primitive
{
    
public:
    Cube();
    void draw();
};

#endif //CUBE_H
