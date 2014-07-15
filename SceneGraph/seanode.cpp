#define GL_GLEXT_PROTOTYPES

#include "seanode.h"
#include <iostream>
#include <GL/gl.h>


SeaNode::SeaNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{

}


SeaNode::~SeaNode() {

}
