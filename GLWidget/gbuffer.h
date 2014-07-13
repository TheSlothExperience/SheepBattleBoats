#ifndef GBUFFER_H
#define GBUFFER_H

#include<GL/gl.h>
#include<GL/glu.h>
#include<QOpenGLShader>

class GBuffer
{
public:
    GBuffer();

    enum GBUFFER_TEXTURE_TYPE{
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

    bool Init(unsigned int windowWidth, unsigned int windowHeight);

    void startFrame();
    void bindGeometryPass();
    void bindStencilPass();
    void bindLightPass( QOpenGLShaderProgram *canvasProgram);
    void bindFinalPass();
//    void setReadBuffer(GBUFFER_TEXTURE_TYPE texType);

private:
    GLuint m_fbo;
    GLuint m_textures[4];
    GLuint m_depthTexture;
    GLuint m_finalTexture;


};

#endif // GBUFFER_H
