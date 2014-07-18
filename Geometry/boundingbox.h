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
    QString getString();
};

#endif // BOUNDINGBOX_H
