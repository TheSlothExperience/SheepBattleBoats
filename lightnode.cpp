#include "lightnode.h"

LightNode::LightNode(Primitive *p, std::string name)
    : SceneGraph(p, name)
{
    
}

QVector3D LightNode::getPosition() {
    return translation;
}
