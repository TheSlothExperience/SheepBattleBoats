#define GL_GLEXT_PROTOTYPES

#include "heightmapnode.h"
#include <iostream>

HeightMapNode::HeightMapNode(HeightMap *p, std::string name)
	: SceneGraph(p, name)
{
	this->heightMap = p;

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glPixelStorei(GL_PACK_ALIGNMENT, 2);
	glGenTextures(1, &heightMapTexLocation);
	glBindTexture(GL_TEXTURE_2D, heightMapTexLocation);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

HeightMapNode::~HeightMapNode() {
	glDeleteTextures(1, &heightMapTexLocation);
}

void HeightMapNode::loadHeightMap(int width, int height, unsigned short* raw) {
	glBindTexture(GL_TEXTURE_2D, heightMapTexLocation);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glPixelStorei(GL_PACK_ALIGNMENT, 2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED,
	             GL_UNSIGNED_SHORT, (void*) raw);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] raw;
}

void HeightMapNode::drawGrid(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc) {
    
	MVStack.push(MVStack.top());  
	MVStack.top().translate(this->translation);
    
	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;
    
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, MVStack.top().constData());
    
	this->heightMap->drawGrid();
    
	MVStack.pop();
}
