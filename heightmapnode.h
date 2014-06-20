#ifndef HEIGHTMAPNODE_H
#define HEIGHTMAPNODE_H

#include "heightmap.h"
#include "scenegraph.h"

#include <vector>

class HeightMapNode : public SceneGraph
{
private:
	HeightMap *heightMap;
	GLuint heightMapTexLocation;
	unsigned short* heightMapData;

	int terrainWidth;
	int terrainHeight;
	int gridWidth;
	int gridHeight;

	std::vector<GLuint> factures;
public:
	HeightMapNode(HeightMap *h, std::string name);
	~HeightMapNode();

	void drawGrid(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);

	void loadHeightMap(int width, int height, unsigned short* raw);
	void loadFacture(int width, int height, unsigned char* raw);

	GLuint getHeightMapLocation() {return heightMapTexLocation;}

	std::vector<GLuint> getFactures() {return factures;}

	float getHeightAt(float x, float y);
};


#endif
