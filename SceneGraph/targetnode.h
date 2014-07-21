#ifndef TARGETNODE_H
#define TARGETNODE_H

#include"scenegraph.h"
#include"boundingbox.h"


class TargetNode: public SceneGraph
{

public:
    TargetNode(Primitive *p, std::string name);
    ~TargetNode();
    void draw();
    void testCollisions(SceneGraph);

    void move();

    void exeObjBehaviour();
    BoundingBox* getBB();
    void reactToCollision();
    QVector3D getPosition();
};


#endif // TARGETNODE_H
