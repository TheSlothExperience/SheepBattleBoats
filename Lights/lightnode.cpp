#define GL_GLEXT_PROTOTYPES

#include "glwidgetcontext.h"
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


void LightNode::lightPass(std::stack<QMatrix4x4> &MVStack, SceneGraph *root) {
	QOpenGLShaderProgram *shader = Shaders::storeDepthProgram;
	Shaders::bind(shader);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO());

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT1};
	glDrawBuffers(1, DrawBuffers);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0, shadowWidth(), shadowHeight());

	MVStack.push(MVStack.top());
	MVStack.top() *= lightView();

	root->drawGeometry(MVStack
	                  , lightView()
	                  , perspectiveMatrix()
	                  , shader);

	MVStack.pop();

	//Recreate the mipmaps
	glBindTexture(GL_TEXTURE_2D, shadowMoments());
	glGenerateMipmap(GL_TEXTURE_2D);

	//Release and relax, brah
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shaders::release(shader);
}
