#ifndef LEVELOBJECTNODE_H
#define LEVELOBJECTNODE_H

#include<scenegraph.h>
#include<levelobject.h>
#include<boundingbox.h>

class SheepNode : public SceneGraph
{
private :
	//Movement Parameters
	QVector3D v; //Positionsänderungen pro GameTick
	float rotationAngle;//Winkel der Rotation pro GameTick
	bool moving;
	const float MAX_ROTATION=0.05;
	const QVector3D MAX_VELOCITY=QVector3D(0.0,0.0,0.3);
public:

	SheepNode(Primitive *p, std::string name);
	~SheepNode();
	void testCollisions(SheepNode);

	void move();

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
	void exeObjBehaviour();
	BoundingBox* getBB();
	void reactToCollision();
};

#endif // LEVELOBJECTNODE_H
