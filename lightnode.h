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

    QVector4D lightColor;

public:
    LightNode(Primitive *p, std::string name);
    ~LightNode();

    QVector3D getPosition();
    QVector4D getLightColor() {return lightColor;}
    void setLightColor(QVector4D lc) {lightColor = lc;}
    
    void changeColor(float r, float g, float b, float a);
};

#endif
