#define GL_GLEXT_PROTOTYPES

#include "lightnode.h"
#include <iostream>
#include <GL/gl.h>

LightNode::LightNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{
}

LightNode::~LightNode() {
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


void LightNode::lightPass() {

}
