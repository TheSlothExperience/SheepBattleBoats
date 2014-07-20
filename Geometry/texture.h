#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/gl.h>

class Texture
{
public:
    Texture();
    Texture(GLenum textureTarget, const std::string& fileName);
    bool load();
    void bind(GLenum textureUnit);
	GLuint getTexLocation();

private:
    std::string fileName;
    GLenum textureTarget;
    GLuint textureObj;
};

#endif // TEXTURE_H
