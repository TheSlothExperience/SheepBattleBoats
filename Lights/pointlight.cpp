#define GL_GLEXT_PROTOTYPES

#include "pointlight.h"
#include "glwidgetcontext.h"

#include <iostream>
#include <GL/gl.h>

PointLight::PointLight(Primitive *p, std::string name)
	: LightNode(p, name)
{
	std::cout << "Creating PointLight" << std::endl;
	m_shadowWidth = 512;
	m_shadowHeight = 512;

	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
	color[3] = 1.0;

	//Create buffer for showmap
	glEnable(GL_DEPTH_TEST);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_shadowWidth, m_shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowMap, 0);

	glGenTextures(1, &moments);
	glBindTexture(GL_TEXTURE_2D, moments);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_shadowWidth, m_shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, moments, 0);

	glGenTextures(1, &momentsTemp);
	glBindTexture(GL_TEXTURE_2D, momentsTemp);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_shadowWidth, m_shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, momentsTemp, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "SOMETHING WENT WRONG IN THE SHADOWMAP FBO, CHIEF!!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

PointLight::~PointLight() {
}

QVector3D PointLight::getPosition() {
	return translation;
}

void PointLight::changeColor(float r, float g, float b, float a) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}


void PointLight::lightPass(std::stack<QMatrix4x4> &MVStack, SceneGraph *root) {
	QOpenGLShaderProgram *shader = Shaders::storeDualParaboloidProgram;
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
