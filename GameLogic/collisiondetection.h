#ifndef COLLISIONDETECION_H
#define COLLISIONDETECION_H

#include<boundingbox.h>

class CollisionDetecion
{
public:
    CollisionDetecion();
    static int isCollision(BoundingBox*,BoundingBox*);
};

#endif // COLLISIONDETECION_H
