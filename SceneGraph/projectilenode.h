#ifndef PROJECTILENODE_H
#define PROJECTILENODE_H

#include <scenegraph.h>

class ProjectileNode: public SceneGraph
{
private:
    const int MAX_TICKSTOLIVE=100;
    int livedTicks=0;
    const float gravity=-1.0;
    QVector3D shootDir;
    QVector3D startPos;

public:
    ProjectileNode(QVector3D startPos,QVector3D shootingDir,Primitive *p, std::string name);
    ~ProjectileNode();
    void draw();
    void exeObjBehaviour();

};

#endif // PROJECTILENODE_H
