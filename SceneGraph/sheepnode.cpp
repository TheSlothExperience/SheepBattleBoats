#include <sheepnode.h>

SheepNode::SheepNode(Primitive *p, std::string name, QQuaternion rotationOffset)
	:SceneGraph(p, name, rotationOffset)
{
	this->name=name;
	v=QVector3D(0.0,0.0,0.0);
	rotationAngle=0;
	this->moving=false;
}

SheepNode::~SheepNode(){

}

void SheepNode::testCollisions(SheepNode secondObject){
}

BoundingBox* SheepNode::getBB(){
	BoundingBox* a=primitive->getBB();
	a->position=translation;

	return a;
}

//Movement of other Objects
void SheepNode::move(){
}

void SheepNode::increaseVelocity(QVector3D vTobeAdded){
	v+=vTobeAdded;
	if(v.z()>MAX_VELOCITY.z()){
		v=MAX_VELOCITY;
	}
	if(v.z()<-MAX_VELOCITY.z()){
		v=-MAX_VELOCITY;
	}
}

void SheepNode::changeSteeringValue(float angleTobeAdded){
	rotationAngle+=angleTobeAdded;
	if(rotationAngle>MAX_ROTATION){
		rotationAngle=MAX_ROTATION;
	}

	if(rotationAngle<-MAX_ROTATION){
		rotationAngle=-MAX_ROTATION;
	}
}

void SheepNode::stopVelocity(){
	if(v.z()>0.003){
		increaseVelocity(QVector3D(0.0,0.0,-0.0030));
	}else if(v.z()<-0.003){
		increaseVelocity(QVector3D(0.0,0.0,0.0030));
	}
	else{
		setVelocity(QVector3D(0.0,0.0,0.0));
	}
}

void SheepNode::stopRotating(){
	if(rotationAngle>0.02){
		changeSteeringValue(-0.02);
	}else if(rotationAngle<-0.02){
		changeSteeringValue(0.02);
	}
	else{
		rotationAngle=0;
	}
}

void SheepNode::exeObjBehaviour(){
	move();
}

void SheepNode::reactToCollision(){
	setDeadmark(true);
}
