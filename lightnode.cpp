#include "lightnode.h"

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
