#ifndef VOLUME_H
#define VOLUME_H

#include <GL/gl.h>

#include "primitive.h"

class Volume : public Primitive
{
private:
    GLuint texcoordsBufferObject;
public:
    Volume();
    Volume(float ax, float ay, float az);
    void drawVolume();
    void drawBB();
    void draw();
};



#endif
