#ifndef SPHERE_H
#define SPHERE_H

#include <GL/gl.h>

#include "primitive.h"

class Sphere : public Primitive
{
private:
	GLuint indexBufferObject;
	int count;
public:
	Sphere(int tesselationLevel);
	~Sphere();
	void draw();
};


#endif
