#define GL_GLEXT_PROTOTYPES

#include "heightmapnode.h"
#include <iostream>

const float SHORT_MAX = 65535;
const float heightScale = 100.0 / SHORT_MAX;

HeightMapNode::HeightMapNode(HeightMap *p, std::string name)
	: SceneGraph(p, name)
{
	this->heightMap = p;
    this->heightMapData = NULL;


	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glPixelStorei(GL_PACK_ALIGNMENT, 2);
	glGenTextures(1, &heightMapTexLocation);
	glBindTexture(GL_TEXTURE_2D, heightMapTexLocation);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

HeightMapNode::~HeightMapNode() {
	glDeleteTextures(1, &heightMapTexLocation);
	delete[] heightMapData;
}

float HeightMapNode::getHeightAt(float x, float y) {
	if(fabs(x) < 25 && fabs(y) < 25 && heightMapData != NULL) {
		float x_tc = x / 50.0;
		float y_tc = y / 50.0;

		x_tc += 0.5;
		y_tc += 0.5; // Coords now in [0, 1)

		int x_idx = x_tc * (float) terrainWidth;
		int y_idx = y_tc * (float) terrainHeight; // In [0, terrain{W|H})

		return heightScale * (float)heightMapData[y_idx * terrainWidth + x_idx];
	}
	return 0;
}

void HeightMapNode::loadHeightMap(int width, int height, unsigned short* raw) {
	if(heightMapData != NULL) delete[] heightMapData;
	heightMapData = raw;

	this->terrainWidth = width;
	this->terrainHeight = height;

	glBindTexture(GL_TEXTURE_2D, heightMapTexLocation);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glPixelStorei(GL_PACK_ALIGNMENT, 2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED,
	             GL_UNSIGNED_SHORT, (void*) raw);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void HeightMapNode::loadFacture(int width, int height, unsigned char* raw) {
	GLuint loc;

	glGenTextures(1, &loc);
	glBindTexture(GL_TEXTURE_2D, loc);;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, (void*) raw);

	glBindTexture(GL_TEXTURE_2D, 0);

	factures.push_back(loc);
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
