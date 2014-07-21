#include "targetnode.h"
#include "gamestate.h"

TargetNode::TargetNode(Primitive *p, std::string name)
    :SceneGraph(p, name)
{
    this->name=name;
}

TargetNode::~TargetNode(){
}


void TargetNode::draw(){
    primitive->draw();
}

//void TargetNode::testCollisions(TargetNode secondObject){
//}

BoundingBox* TargetNode::getBB(){
    BoundingBox* a=primitive->getBB();
    a->position=translation;
    return a;

}


void TargetNode::exeObjBehaviour(){
}

void TargetNode::reactToCollision(){
    setDeadmark(true);
    GameState::addPoints(100);
}

QVector3D TargetNode::getPosition(){
    return this->getBB()->position;
}
