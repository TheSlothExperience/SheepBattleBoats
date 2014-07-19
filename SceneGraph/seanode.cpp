#define GL_GLEXT_PROTOTYPES

#include "seanode.h"
#include "gbuffer.h"
#include "glwidgetcontext.h"
#include <iostream>
#include <GL/gl.h>


SeaNode::SeaNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{

}


SeaNode::~SeaNode() {
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack) {
	//draw(MVStack, Shaders::shaderProgram);
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack, QOpenGLShaderProgram *shader) {
}
