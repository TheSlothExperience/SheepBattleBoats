#define GL_GLEXT_PROTOTYPES
#include "gbuffer.h"
#include <iostream>

GBuffer* GBuffer::m_activeGBuffer = NULL;

GBuffer* GBuffer::activeGBuffer() {
	return m_activeGBuffer;
}

GBuffer::GBuffer()
{
    m_fbo=0;
    m_depthTexture=0;
    m_finalTexture=0;

    numTextures = 4;
    numTemps = 3;
}

bool GBuffer::Init(unsigned int windowWidth, unsigned int windowHeight){
	m_activeGBuffer = this;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Erzeuge 5 Texturen
	glGenTextures(numTextures, m_textures);

    //Erzeuge 5 temp texture
    glGenTextures(numTemps, m_tempTextures);

	// Depth
	glGenTextures(1, &m_depthTexture);

	//Erzeuge die Finale Textur
	glGenTextures(1,&m_finalTexture);

	for (int i = 0 ; i < numTextures ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		// Dummy-Aufruf, damit OpenGL weiß welches Texturformat wir
		// anlegen möchten (alternativ auch GL_RGBA16F)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		//Bind FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
		                       GL_TEXTURE_2D, m_textures[i], 0);
	}

    for (int i = 0 ; i < numTemps ; i++) {
        glBindTexture(GL_TEXTURE_2D, m_tempTextures[i]);

        // Dummy-Aufruf, damit OpenGL weiß welches Texturformat wir
        // anlegen möchten (alternativ auch GL_RGBA16F)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //Bind FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i + numTextures,
                               GL_TEXTURE_2D, m_tempTextures[i], 0);
    }

	// depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0,
	             GL_DEPTH_COMPONENT, GL_FLOAT,NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	//Finale Textur
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0,
	             GL_RGBA, GL_FLOAT,NULL);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numTextures + numTemps, GL_TEXTURE_2D, m_finalTexture, 0);

       GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

       if (Status != GL_FRAMEBUFFER_COMPLETE) {
           printf("FB error, status: 0x%x\n", Status);
           return false;
       }

       // restore default FBO
       glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

       return true;
}


void GBuffer::startFrame(){
	m_activeGBuffer = this;
	glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);

	GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0,
	                      GL_COLOR_ATTACHMENT1,
	                      GL_COLOR_ATTACHMENT2,
	                      GL_COLOR_ATTACHMENT3,
	                      GL_COLOR_ATTACHMENT4,
	                      GL_COLOR_ATTACHMENT5,
	                      GL_COLOR_ATTACHMENT6,
	                      GL_COLOR_ATTACHMENT7};

	glDrawBuffers(6,drawBuffers);
	glClearDepth(1.0);
	glClearColor(8.0f, 8.0f, 8.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void GBuffer::bindGeometryPass(){
	m_activeGBuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);

    GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0,
                          GL_COLOR_ATTACHMENT1,
                          GL_COLOR_ATTACHMENT2,
                          GL_COLOR_ATTACHMENT3};

    glDrawBuffers(4,drawBuffers);
}

void GBuffer::bindStencilPass(){
	m_activeGBuffer = this;
}

void GBuffer::bindLightPass( QOpenGLShaderProgram *lightPassProgram){
	m_activeGBuffer = this;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0,0, windowWidth, windowHeight);

    glActiveTexture(GL_TEXTURE0);
        glUniform1i(lightPassProgram->uniformLocation("posTexture"),0);
    glBindTexture(GL_TEXTURE_2D,m_textures[0]);

    glActiveTexture(GL_TEXTURE1);
        glUniform1i(lightPassProgram->uniformLocation("colorTexture"),1);
    glBindTexture(GL_TEXTURE_2D,m_textures[1]);

    glActiveTexture(GL_TEXTURE2);
        glUniform1i(lightPassProgram->uniformLocation("normalTexture"),2);
    glBindTexture(GL_TEXTURE_2D,m_textures[2]);

    glActiveTexture(GL_TEXTURE3);
        glUniform1i(lightPassProgram->uniformLocation("depthCoordTexture"),3);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);

    glActiveTexture(GL_TEXTURE4);
        glUniform1i(lightPassProgram->uniformLocation("depthTexture"),4);
    glBindTexture(GL_TEXTURE_2D,m_textures[3]);

    GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0 + numTemps + numTextures};

    glDrawBuffers(1,drawBuffers);
}

void GBuffer::drawToFinal() {
	m_activeGBuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
	glViewport(0,0, windowWidth, windowHeight);
    GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0 + numTemps + numTextures};
    glDrawBuffers(1,drawBuffers);
}

void GBuffer::bindFinalPass(QOpenGLShaderProgram *canvasProgram){
	m_activeGBuffer = this;

    //Fertig mit schreiben
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glActiveTexture(GL_TEXTURE0);
        glUniform1i(canvasProgram->uniformLocation("scene"),0);
    glBindTexture(GL_TEXTURE_2D,m_finalTexture);
}

void GBuffer::tempTexture(int i){
    m_activeGBuffer = this;
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glViewport(0,0, windowWidth, windowHeight);
    GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0 + i + numTextures};
    glDrawBuffers(1,drawBuffers);
}

GLuint GBuffer::getTempTexture(int i){

    return m_tempTextures[i];
}
