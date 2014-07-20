#ifndef PROJECTILENODE_H
#define PROJECTILENODE_H

#include <scenegraph.h>

class ProjectileNode: public SceneGraph
{
private:
    const int MAX_TICKSTOLIVE=500;
    float livedTicks=0.0;
    const float gravity=-2.0;
    QVector3D shootDir;
    QVector3D startPos;
    void setNewPosition(QVector3D newPos);

public:
    ProjectileNode(QVector3D startPos,QVector3D shootingDir,Primitive *p, std::string name);
    ~ProjectileNode();
    void draw();
    void exeObjBehaviour();
    BoundingBox* getBB();
    void reactToCollision();

};

#endif // PROJECTILENODE_H
