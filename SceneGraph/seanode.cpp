#define GL_GLEXT_PROTOTYPES

#include "seanode.h"
#include "gbuffer.h"
#include "glwidgetcontext.h"
#include "scene.h"
#include <iostream>
#include <GL/gl.h>


SeaNode::SeaNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{

}


SeaNode::~SeaNode() {
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix) {
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
	GBuffer::activeGBuffer()->drawToFinal();
	Shaders::bind(Shaders::phongProgram);
	Scene::passLights(cameraMatrix, Shaders::phongProgram);
	MVStack.push(MVStack.top());

	MVStack.top().translate(this->translation);

	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;

	//If the node is a leaf, draw its contents
	if(leaf) {
		glUniformMatrix4fv(Shaders::phongProgram->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
		glUniformMatrix4fv(Shaders::phongProgram->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());
		glUniformMatrix4fv(Shaders::phongProgram->uniformLocation("normalMatrix"), 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
		int r = (id & 0x000000FF) >>  0;
		int g = (id & 0x0000FF00) >>  8;
		int b = (id & 0x00FF0000) >> 16;
		glUniform4f(Shaders::phongProgram->uniformLocation("id"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

		glUniform4fv(Shaders::phongProgram->uniformLocation("color"), 1, color);

		this->primitive->draw();
	} else {
		//Else, recurse into its children
		std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->draw(MVStack, cameraMatrix, projectionMatrix, shader);});
	}

	MVStack.pop();
	Shaders::release(Shaders::phongProgram);
}

void SeaNode::drawGeometry(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
}
