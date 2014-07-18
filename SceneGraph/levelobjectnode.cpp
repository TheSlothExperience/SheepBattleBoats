#include <levelobjectnode.h>

LevelObjectNode::LevelObjectNode(LevelObject *p, std::string name)
    :SceneGraph(p, name)
{
    lvlObj = p;
    this->name=name;
}

LevelObjectNode::~LevelObjectNode(){

}


void LevelObjectNode::draw(){

}

void LevelObjectNode::testCollisions(LevelObjectNode secondObject){

//    if(leaf) {
//    qDebug()<<"testCollision in levelnode";
//    } else {
//        //Else, recurse into its children
//        std::for_each(children.begin(), children.end(), [](SceneGraph *s){s->testCollisions();});
//    }
}

BoundingBox* LevelObjectNode::getBB(){
    BoundingBox* a=lvlObj->getBB();
    qDebug()<<QString::number(translation.x());
    a->position=translation;

    return a;

}


