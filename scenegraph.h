#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"
#include "cube.h"

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
    SceneGraph(Primitive *p, std::string name);
    
    SceneGraph* parent();

    SceneGraph* child(int row);
    int childCount();
    int columnCount();
    QVariant data(int column);
    bool setData(int column, const QVariant &value);
    bool removeChildren(int position, int count);
    int row();

    void setParent(SceneGraph *s);

    void translate(double x, double y, double z);
    void rotate(QQuaternion q);

    std::string getName() {return name;};
    void setName(std::string name);
    bool isLeaf() {return leaf;};
    SceneGraph* add(Primitive *p);
    SceneGraph* add(SceneGraph *s);
    void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc);
};

#endif //SCENE_H
