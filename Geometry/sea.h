#ifndef SEA_H
#define SEA_H

#include <GL/gl.h>

#include "primitive.h"

class Sea : public Primitive
{

public:
	Sea();
	~Sea();
	void draw();

private:
	int seaWidth;
	int seaHeight;
	int tessellation;
	int numVertices;
};


#endif
