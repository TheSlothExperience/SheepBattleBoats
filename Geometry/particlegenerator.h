#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include <GL/gl.h>
#include "primitive.h"

class ParticleGenerator: public Primitive
{
public:
    ParticleGenerator();
    ~ParticleGenerator();
    void draw();
};

#endif // PARTICLEGENERATOR_H
