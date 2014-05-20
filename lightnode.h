#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"
#include "scenegraph.h"


class LightNode : public SceneGraph
{
private:
    std::string name;
    bool leaf = false;
    int id;
    
    QQuaternion rotation;
    QVector3D translation;

    Primitive *primitive;
    SceneGraph *parentNode;

    std::vector<SceneGraph* > children;

    QVector4D lightColor;

public:
    LightNode(Primitive *p, std::string name);

    QVector4D getPosition();
    QVector4D getLightColor() {return lightColor;}
    void setLightColor(QVector4D lc) {lightColor = lc;}
    
    //void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc, GLuint idLoc);
};


#endif
