#include "lightnode.h"
#include <iostream>

LightNode::LightNode(Primitive *p, std::string name)
    : SceneGraph(p, name)
{
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    color[3] = 1.0;
}

QVector3D LightNode::getPosition() {
    return translation;
}

void LightNode::changeColor(float r, float g, float b, float a) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}
