#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "primitive.h"

class Projectile:public Primitive
{
public:
    Projectile();
    ~Projectile();
    void draw();
};

#endif // PROJECTILE_H
