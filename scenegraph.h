#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <stack>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"

class SceneGraph
{
private:
    bool leaf = false;

    QQuaternion rotation;
    QVector3D translation;

    Primitive *primitive;

    std::vector<SceneGraph > children;

public:
    SceneGraph(bool isleaf = false);
    SceneGraph(Primitive *p);
    
    bool isLeaf();
    void add(Primitive *p);
    void add(SceneGraph &s);
    void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc);
};

#endif //SCENE_H
