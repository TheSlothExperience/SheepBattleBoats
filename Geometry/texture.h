#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/gl.h>
#include <QImage>

class Texture
{
public:
    Texture();
    Texture(GLenum textureTarget, const std::string& fileName);
    bool load();
    void bind(GLenum textureUnit);

private:
    std::string fileName;
    GLenum textureTarget;
    GLuint textureObj;
    //QImage* image;

};

#endif // TEXTURE_H
