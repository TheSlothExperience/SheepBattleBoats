#include "lightnode.h"

LightNode::LightNode(Primitive *p, std::string name)
    : SceneGraph(p, name)
{
    
}

QVector4D LightNode::getPosition() {
    return QVector4D(translation, 1.0);
}
