#include "texture.h"

Texture::Texture(){}

Texture::Texture(GLenum textureTarget, const std::string& fileName){

    this->textureTarget = textureTarget;
    this->fileName = fileName;
    this->image = NULL;

}

bool Texture::load(){

    //load that shit somehow with image magic
    if(!fileName.empty()){
        //image = new QImage(fileName, "JPG");
        //m_pImage->write(&m_blob, "RGBA");
    }else {
        std::cout << "Error loading texture '" << fileName << std::endl;
        return false;
    }
      glGenTextures(1, &textureObj);
      glBindTexture(textureTarget, textureObj);
      glTexImage2D(textureTarget, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());
      glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(textureTarget, textureObj);
}