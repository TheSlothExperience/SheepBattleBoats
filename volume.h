#ifndef VOLUME_H
#define VOLUME_H

#include <GL/gl.h>

#include "primitive.h"

class Volume : public Primitive
{
    
public:
    Volume();
    void drawVolume();
    void drawBB();
    void draw();
};



#endif
