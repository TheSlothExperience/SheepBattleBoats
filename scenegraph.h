#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"

class SceneGraph
{
private:
    std::string name;
    bool leaf = false;

    QQuaternion rotation;
    QVector3D translation;

    Primitive *primitive;
    SceneGraph *parentNode;

    std::vector<SceneGraph* > children;

public:
    SceneGraph(bool isleaf = false, SceneGraph *parent = 0);
    SceneGraph(Primitive *p, SceneGraph *parent = 0);
    
    SceneGraph* parent();

    SceneGraph* child(int row);
    int childCount();
    int columnCount();
    QVariant data(int column);
    int row();

    void setParent(SceneGraph *s);
    
    bool isLeaf();
    void add(Primitive *p);
    void add(SceneGraph *s);
    void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc);
};

#endif //SCENE_H
