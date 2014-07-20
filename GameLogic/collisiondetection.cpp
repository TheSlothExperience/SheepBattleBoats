#include "collisiondetection.h"
#include <QtGui>
#include<iostream>
CollisionDetecion::CollisionDetecion()
{
}

int CollisionDetecion::isCollision(BoundingBox* bb1, BoundingBox *bb2){

//    qDebug()<<"bb1:\n"+bb1->getString();
//    qDebug()<<"bb2:\n"+bb2->getString();
//    std::cout<<"min:" << bb2->min.x()<< " "<< bb2->min.y()<< " "<< bb2->min.z()<< " " <<std::endl;

    if (bb1->max[0]+bb1->position.x() < bb2->min[0]+bb2->position.x() || bb1->min[0]+bb1->position.x() > bb2->max[0]+bb2->position.x())return 0;

    if (bb1->max[1]+bb1->position.y() < bb2->min[1]+bb2->position.y() || bb1->min[1]+bb1->position.y() > bb2->max[1]+bb2->position.y()) return 0;

    if (bb1->max[2]+bb1->position.z() < bb2->min[2]+bb2->position.z() || bb1->min[2]+bb1->position.z() > bb2->max[2]+bb2->position.z()) return 0;

//    std::cout<<bb1->position.x;
    qDebug()<<"BOOM between "
              +QString::number(bb1->position.x())+","
              +QString::number(bb1->position.y())+","
              +QString::number(bb1->position.z())+
              "  and  "
              +QString::number(bb2->position.x())+","
              +QString::number(bb2->position.y())+","
              +QString::number(bb2->position.z());
    //Wenn es soweit kommt, dann gibt es eine Schnitt
    return 1;
}
