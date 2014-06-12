#define GL_GLEXT_PROTOTYPES

#include "heightmap.h"

HeightMap::HeightMap() {
	static GLfloat vertices[] = {
		-50, 0, -50,
		-50, 0, 50,
		50, 0, -50,
		50, 0, 50
	};

	static GLfloat normals[] = {
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};
    
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

HeightMap::~HeightMap() {
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &normalBufferObject);
}

void HeightMap::draw() {
	
}

void HeightMap::drawGrid() {
	//Bind the arrays to the vao
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
