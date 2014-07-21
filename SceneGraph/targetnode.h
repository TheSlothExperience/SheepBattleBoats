#ifndef TARGETNODE_H
#define TARGETNODE_H

#include"levelobjectnode.h"
#include"scenegraph.h"
#include"levelobject.h"
#include"boundingbox.h"


class TargetNode: public SceneGraph
{

public:
    TargetNode(Primitive *p, std::string name);
    ~TargetNode();
    void draw();
    void testCollisions(LevelObjectNode);


    LevelObject getLvlObj();
    void move();

    void exeObjBehaviour();
    BoundingBox* getBB();
    void reactToCollision();
    QVector3D getPosition();
};


#endif // TARGETNODE_H
