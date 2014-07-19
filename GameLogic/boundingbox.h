#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include<QVector3D>

class BoundingBox
{
private:

public:
    BoundingBox(QVector3D pos,QVector3D min, QVector3D max);
    int intersect(BoundingBox secondBox);
    QVector3D position;
    QVector3D min;
    QVector3D max;
//    QVector3D v1;
//    QVector3D v2;
//    QVector3D v3;
//    QVector3D v4;
//    QVector3D v5;
//    QVector3D v6;
//    QVector3D v7;
//    QVector3D v8;
    QString getString();
};

#endif // BOUNDINGBOX_H
