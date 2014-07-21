#ifndef LEVELOBJECTNODE_H
#define LEVELOBJECTNODE_H

#include<scenegraph.h>
#include<levelobject.h>
#include<boundingbox.h>

class LevelObjectNode :public SceneGraph
{
private :
//    LevelObject* lvlObj;
//    QVector3D a; //acceleration
    //Movement Parameters
    QVector3D v; //Positionsänderungen pro GameTick
    float rotationAngle;//Winkel der Rotation pro GameTick
    bool moving;
    const float MAX_ROTATION=0.05;
    const QVector3D MAX_VELOCITY=QVector3D(0.0,0.0,0.3);
public:

    LevelObjectNode(Primitive *p, std::string name);
    ~LevelObjectNode();
    void draw();
    void testCollisions(LevelObjectNode);


    LevelObject getLvlObj();
    void move();

    //get and set
    QVector3D getVelocity(){return this->v;}
    void setVelocity(QVector3D v){this->v=v;}
    void increaseVelocity(QVector3D v);
    void changeSteeringValue(float angle);
    float getSteeringValue(){return this->rotationAngle;}
    void setSteeringValue(float newAngle){this->rotationAngle=newAngle;}

    bool isMoving(){return this->moving;}
    void setMoving(bool move){this->moving=move;}

    void stopVelocity();
    void stopRotating();
//    void executeObjectBehavior();
    void exeObjBehaviour();
    BoundingBox* getBB();
    void reactToCollision();
    QVector3D getPosition();
};

#endif // LEVELOBJECTNODE_H
