#include "boundingbox.h"
#include<QString>

BoundingBox::BoundingBox(QVector3D  pos,QVector3D  min, QVector3D max)
{
    this->position=pos;
    this->min=min;
    this->max=max;
}

int BoundingBox::intersect(BoundingBox secondBox){
//jetzt woander
}

QString BoundingBox::getString(){
    QString output="";
    output+="min.x="+QString::number(min.x()+position.x());
    output+="\nmin.y="+QString::number(min.y()+position.y());
    output+="\nmin.z="+QString::number(min.z()+position.z());
    output+="\nmax.x="+QString::number(max.x()+position.x());
    output+="\nmax.y="+QString::number(max.y()+position.y());
    output+="\nmax.z="+QString::number(max.z()+position.z());
    return output;


}
