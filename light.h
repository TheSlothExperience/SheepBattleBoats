#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>

#include "primitive.h"


class Light : public Primitive
{
    
public:
    Light();
    void draw();
};

#endif
