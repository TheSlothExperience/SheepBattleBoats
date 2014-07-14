#define GL_GLEXT_PROTOTYPES
#include "gbuffer.h"



GBuffer::GBuffer()
{
    m_fbo=0;
    m_depthTexture=0;
    m_finalTexture=0;
}

bool GBuffer::Init(unsigned int windowWidth, unsigned int windowHeight){

	// Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Erzeuge 5 Texturen
	glGenTextures(5, m_textures);

	//Erzeuge die Tiefen Textur
	glGenTextures(1, &m_depthTexture);

	//Erzeuge die Finale Textur
	glGenTextures(1,&m_finalTexture);

	for (int i = 0 ; i < 5 ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		// Dummy-Aufruf, damit OpenGL weiß welches Texturformat wir
		// anlegen möchten (alternativ auch GL_RGBA16F)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		//Bind FBO
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
		                       GL_TEXTURE_2D, m_textures[i], 0);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0,
	             GL_DEPTH_COMPONENT, GL_FLOAT,NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	//Finale Textur
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0,
	             GL_RGB, GL_FLOAT,NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);



//       GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
//       glDrawBuffers(4, DrawBuffers);

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
    glBindBuffer(GL_FRAMEBUFFER,m_fbo);
    GLenum DrawBuffers[5]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4};
    glDrawBuffers(5,DrawBuffers);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void GBuffer::bindGeometryPass(){
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);

    GLenum drawBuffers[]={GL_COLOR_ATTACHMENT0,
                          GL_COLOR_ATTACHMENT1,
                          GL_COLOR_ATTACHMENT2,
                         GL_COLOR_ATTACHMENT3,
                         GL_COLOR_ATTACHMENT4};

    glDrawBuffers(5,drawBuffers);
}

void GBuffer::bindStencilPass(){

    //Abschalten des Zeichnen der Buffer
//    glDrawBuffers(GL_NONE);
}

void GBuffer::bindLightPass( QOpenGLShaderProgram *lightPassProgram){
//    glDrawBuffer(GL_COLOR_ATTACHMENT4);


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
    glBindTexture(GL_TEXTURE_2D,m_textures[3]);

    glActiveTexture(GL_TEXTURE4);
        glUniform1i(lightPassProgram->uniformLocation("depthTexture2"),4);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);

    glActiveTexture(GL_TEXTURE5);
        glUniform1i(lightPassProgram->uniformLocation("textCoordTexture2"),5);
    glBindTexture(GL_TEXTURE_2D,m_textures[4]);


}

void GBuffer::bindFinalPass(){

    //Fertig mit schreiben
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //Lese aus dem geschriebenen Inhalt im FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER,m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT4);


}

//void GBuffer::BindForReading(){
//    glBindFramebuffer(GL_READ_FRAMEBUFFER,m_fbo);
//}

//void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE texType){
//    glReadBuffer(GL_COLOR_ATTACHMENT0+texType);
//}
