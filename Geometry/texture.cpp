#include "texture.h"

#include <QImage>
#include <QGLWidget>
#include <iostream>

Texture::Texture(){}

Texture::Texture(GLenum textureTarget, const std::string& fileName){
	this->textureTarget = textureTarget;
	this->fileName = fileName;
}

bool Texture::load() {
	if(!fileName.empty()){
		std::cout << fileName << std::endl;
		QImage tex;
		QString fileQT = QString(fileName.c_str());
		tex.load(fileQT);
		std::cout << "Loading facture texture " << fileName << " with: "
		          << "(" << tex.width() << ", " << tex.height() << ")" << std::endl;
		tex = QGLWidget::convertToGLFormat(tex);

		glGenTextures(1, &textureObj);
		glBindTexture(textureTarget, textureObj);
		glTexImage2D(textureTarget, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(textureTarget, 0);
		return true;
	} else {
		std::cout << "Error loading texture '" << fileName << std::endl;
		return false;
	}
}

GLuint Texture::getTexLocation() {
	return textureObj;
}

void Texture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObj);
}
