#include "collisiondetection.h"
#include <QtGui>
#include<iostream>
CollisionDetecion::CollisionDetecion()
{
}

int CollisionDetecion::isCollision(BoundingBox* bb1, BoundingBox *bb2){
    if (bb1->max[0]+bb1->position.x() < bb2->min[0]+bb2->position.x() || bb1->min[0]+bb1->position.x() > bb2->max[0]+bb2->position.x())return 0;

    if (bb1->max[1]+bb1->position.y() < bb2->min[1]+bb2->position.y() || bb1->min[1]+bb1->position.y() > bb2->max[1]+bb2->position.y()) return 0;

    if (bb1->max[2]+bb1->position.z() < bb2->min[2]+bb2->position.z() || bb1->min[2]+bb1->position.z() > bb2->max[2]+bb2->position.z()) return 0;

    return 1;
}
