#include "boundingbox.h"
#include<QString>

BoundingBox::BoundingBox(QVector3D  pos,QVector3D  min, QVector3D max)
{
    this->position=pos;
    this->min=min;
    this->max=max;
//    v1=QVector3D(min.x(),min.y(),min.z()); //luh
//    v2=QVector3D(min.x(),max.y(),min.z()); //loh
//    v3=QVector3D(max.x(),max.y(),min.z()); //roh
//    v4=QVector3D(max.x(),min.y(),min.z()); //ruh
//    v5=QVector3D(max.x(),min.y(),max.z()); //ruv
//    v6=QVector3D(max.x(),max.y(),max.z()); //rov
//    v7=QVector3D(min.x(),max.y(),max.z()); //lov
//    v8=QVector3D(min.x(),min.y(),max.z()); //luv


}

int BoundingBox::intersect(BoundingBox secondBox){
//jetzt woander
}

QString BoundingBox::getString(){
    QString output="";
//    output+="min.x="+QString::number(min.x()+position.x());
//    output+="\nmin.y="+QString::number(min.y()+position.y());
//    output+="\nmin.z="+QString::number(min.z()+position.z());
//    output+="\nmax.x="+QString::number(max.x()+position.x());
//    output+="\nmax.y="+QString::number(max.y()+position.y());
//    output+="\nmax.z="+QString::number(max.z()+position.z());
    return output;


}

void BoundingBox::setNewPosition(QVector3D newPos){
    this->position=newPos;
}
