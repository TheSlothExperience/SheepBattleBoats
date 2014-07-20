#include "projectilenode.h"

ProjectileNode::ProjectileNode(QVector3D startPos,QVector3D shootingDir,Primitive *p, std::string name)
    :SceneGraph(p, name)
{
    this->startPos=startPos;
    this->name=name;
    this->shootDir=shootingDir;
    qDebug()<<"sDirX: "+QString::number(shootDir.x())+"sDirY: "+QString::number(shootDir.y())+"sDirZ: "+QString::number(shootDir.z());
}

ProjectileNode::~ProjectileNode(){
}

void ProjectileNode::draw(){
    primitive->draw();

}

void ProjectileNode::exeObjBehaviour(){
//    qDebug()<<"exeObjBehaviour in projectileNode.";
    float t= livedTicks;
    if(livedTicks>=MAX_TICKSTOLIVE||translation.y()<-10.0){
//        qDebug()<<"Projectile shouldbe dead";
    }else{
        livedTicks+=0.1;
        translation=QVector3D(shootDir.x()*t+startPos.x(),
                              gravity*t*t+shootDir.y()*t+startPos.y(),
                              shootDir.z()*t+startPos.z());
//        setNewBBPosition(translation);

    }
}

BoundingBox* ProjectileNode::getBB(){
    BoundingBox* a=primitive->getBB();
//    qDebug()<<QString::number(translation.x());
    a->position=translation;

    return a;
}

void ProjectileNode::setNewPosition(QVector3D newPos){
    this->getBB()->setNewPosition(newPos);
}

void ProjectileNode::reactToCollision(){
    qDebug()<<"deleting row " << row();


    setDeadmark(true);
//    emit deleteNode(this);
}
