#ifndef HEIGHTMAPNODE_H
#define HEIGHTMAPNODE_H

#include "heightmap.h"
#include "scenegraph.h"

#include <vector>

#define SHORT_MAX 65535.0;

class HeightMapNode : public SceneGraph
{
private:
	HeightMap *heightMap;
	GLuint heightMapTexLocation;
	unsigned short* heightMapData;

	bool showMeshp = false;
	bool slopeMixingp = false;
	int terrainWidth;
	int terrainHeight;
	int gridWidth;
	int gridHeight;
	float maxVal;

	float heightScale = 100 / SHORT_MAX;
	float terrainSize = 1.0;

	std::vector<GLuint> factures;
	std::vector<int> specularities;
public:
	HeightMapNode(HeightMap *h, std::string name);
	~HeightMapNode();

	void drawGrid(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);

	void loadHeightMap(int width, int height, unsigned short* raw);
	void loadFacture(int width, int height, unsigned char* raw, int specularity);

	GLuint getHeightMapLocation() {return heightMapTexLocation;}

	std::vector<GLuint> getFactureLocations() {return factures;}
	std::vector<int> getSpecularities() {return specularities;}

	float getHeightAt(float x, float y);
	void setMaximumValue(unsigned short val);
	float getMaximumHeight();

	void setHeightScale(float scale) {this->heightScale = scale / SHORT_MAX;}
	float getHeightScale() {return heightScale;}

	void setTerrainSize(float size) {this->terrainSize = size;}
	float getTerrainSize() {return terrainSize;}

	bool showMesh() {return showMeshp;}
	void setShowMesh(bool show) {showMeshp = show;}

	bool slopeMixing() {return slopeMixingp;}
	void setSlopeMixing(bool show) {slopeMixingp = show;}
};


#endif
