#define GL_GLEXT_PROTOTYPES

#include "seanode.h"
#include "Util/Perlin.h"

#include "gbuffer.h"
#include "glwidgetcontext.h"
#include "scene.h"

#include <iostream>
#include <math.h>
#include <GL/gl.h>


SeaNode::SeaNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{
	seaWidth = 200;
	seaHeight = 200;
	periodicity = 200;
	//Create noise texture
	float frequency = 1.0 / pow(2.0, 5);
	noiseData = new GLfloat[seaWidth * seaHeight * (int) periodicity];
	for(int z = 0; z < (int) periodicity; z++) {
		for(int y = 0; y < seaHeight; y++) {
			for(int x = 0; x < seaWidth; x++) {
				noiseData[z * (seaWidth * seaHeight) + y * seaWidth + x] = scaled_raw_noise_3d(0.0, 1.0, x * frequency, y * frequency, z * frequency);
			}
		}
	}

	glGenTextures(1, &noiseTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noiseTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, seaWidth, seaHeight, periodicity, 0, GL_RED, GL_FLOAT, (void*) noiseData);
	glBindTexture(GL_TEXTURE_3D, 0);
}


SeaNode::~SeaNode() {
	glDeleteTextures(1, &noiseTexture);
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix) {
}

void SeaNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
	QOpenGLShaderProgram *sh = Shaders::waterGeometryProgram;
	MVStack.push(MVStack.top());

	MVStack.top().translate(this->translation);

	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;

	//If the node is a leaf, draw its contents
	if(leaf) {
		Shaders::bind(sh);
		glUniformMatrix4fv(sh->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
		glUniformMatrix4fv(sh->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());
		glUniformMatrix4fv(sh->uniformLocation("normalMatrix"), 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
		int r = (id & 0x000000FF) >>  0;
		int g = (id & 0x0000FF00) >>  8;
		int b = (id & 0x00FF0000) >> 16;
		glUniform4f(sh->uniformLocation("id"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

		glUniform4fv(sh->uniformLocation("color"), 1, color);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_3D, noiseTexture);
		glUniform1i(sh->uniformLocation("noiseTexture"), 5);

		glUniform1i(sh->uniformLocation("seaWidth"), seaWidth);
		glUniform1i(sh->uniformLocation("seaHeight"), seaHeight);

		this->primitive->draw();
		Shaders::release(sh);
	} else {
		//Else, recurse into its children
		std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->draw(MVStack, cameraMatrix, projectionMatrix, shader);});
	}

	MVStack.pop();
}

void SeaNode::drawGeometry(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
}
