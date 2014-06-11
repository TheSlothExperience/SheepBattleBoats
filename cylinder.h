#ifndef CYLINDER_H
#define CYLINDER_H

#include <GL/gl.h>

#include "primitive.h"

class Cylinder : public Primitive
{
private:
	GLuint indexBufferObject;
	int count;
public:
	Cylinder(int tesselationLevel);
	~Cylinder();
	void draw();
};


#endif
