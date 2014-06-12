#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <GL/gl.h>

#include "primitive.h"

class HeightMap : public Primitive
{
public:
	HeightMap();
	~HeightMap();

	void drawGrid();
	void draw();
};

#endif
