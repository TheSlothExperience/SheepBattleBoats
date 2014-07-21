#include <levelobjectnode.h>

LevelObjectNode::LevelObjectNode(Primitive *p, std::string name)
    :SceneGraph(p, name)
{
//    lvlObj = p;
    this->name=name;
    v=QVector3D(0.0,0.0,0.0);
    rotationAngle=0;
    this->moving=false;
}

LevelObjectNode::~LevelObjectNode(){
}


void LevelObjectNode::draw(){
    primitive->draw();
}

void LevelObjectNode::testCollisions(LevelObjectNode secondObject){
}

BoundingBox* LevelObjectNode::getBB(){
    BoundingBox* a=primitive->getBB();
    a->position=translation;
    return a;

}

//Movement of other Objects
void LevelObjectNode::move(){
}

void LevelObjectNode::increaseVelocity(QVector3D vTobeAdded){
    v+=vTobeAdded;
    if(v.z()>MAX_VELOCITY.z()){
       v=MAX_VELOCITY;
    }
    if(v.z()<-MAX_VELOCITY.z()){
        v=-MAX_VELOCITY;
    }
}

void LevelObjectNode::changeSteeringValue(float angleTobeAdded){
    rotationAngle+=angleTobeAdded;
    if(rotationAngle>MAX_ROTATION){
        rotationAngle=MAX_ROTATION;
    }

    if(rotationAngle<-MAX_ROTATION){
        rotationAngle=-MAX_ROTATION;
    }
}

void LevelObjectNode::stopVelocity(){
    if(v.z()>0.003){
        increaseVelocity(QVector3D(0.0,0.0,-0.0030));
    }else if(v.z()<-0.003){
        increaseVelocity(QVector3D(0.0,0.0,0.0030));
    }
    else{
        setVelocity(QVector3D(0.0,0.0,0.0));
    }
}

void LevelObjectNode::stopRotating(){
    if(rotationAngle>0.02){
        changeSteeringValue(-0.02);
    }else if(rotationAngle<-0.02){
        changeSteeringValue(0.02);
    }
    else{
        rotationAngle=0;
    }
}

void LevelObjectNode::exeObjBehaviour(){
    move();
}

void LevelObjectNode::reactToCollision(){
    setDeadmark(true);
}

QVector3D LevelObjectNode::getPosition(){
    return this->getBB()->position;
}
