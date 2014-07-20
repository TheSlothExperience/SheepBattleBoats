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
    if(livedTicks>=MAX_TICKSTOLIVE||translation.y()<0){
        qDebug()<<"Projectile shouldbe dead";
    }else{
        livedTicks++;
        translation=QVector3D(startPos.x(),
                              gravity*livedTicks*livedTicks+shootDir.y()*livedTicks+startPos.y(),
                              shootDir.z()*livedTicks+startPos.z());

    }
}
