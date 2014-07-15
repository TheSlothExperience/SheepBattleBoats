#define GL_GLEXT_PROTOTYPES

#include "sea.h"

#include <iostream>

Sea::Sea() {
	static GLfloat vertices[6 * 50 * 50 * 3];

	static GLfloat normals[6 * 50 * 50 * 3];

	for(int z = 0; z < 50; z++) {
		int z_off = z - 25;
		for(int x = 0; x < 50; x++) {
			int x_off = x - 25;
			vertices[18*(z * 50 + x) + 0] = x_off;
			vertices[18*(z * 50 + x) + 1] = 0.0;
			vertices[18*(z * 50 + x) + 2] = z_off;

			vertices[18*(z * 50 + x) + 3 + 0] = x_off;
			vertices[18*(z * 50 + x) + 3 + 1] = 0.0;
			vertices[18*(z * 50 + x) + 3 + 2] = z_off + 1.0;

			vertices[18*(z * 50 + x) + 6 + 0] = x_off + 1.0;
			vertices[18*(z * 50 + x) + 6 + 1] = 0.0;
			vertices[18*(z * 50 + x) + 6 + 2] = z_off;

			vertices[18*(z * 50 + x) + 9 + 0] = x_off + 1.0;
			vertices[18*(z * 50 + x) + 9 + 1] = 0.0;
			vertices[18*(z * 50 + x) + 9 + 2] = z_off;

			vertices[18*(z * 50 + x) + 12 + 0] = x_off;
			vertices[18*(z * 50 + x) + 12 + 1] = 0.0;
			vertices[18*(z * 50 + x) + 12 + 2] = z_off + 1.0;

			vertices[18*(z * 50 + x) + 15 + 0] = x_off + 1.0;
			vertices[18*(z * 50 + x) + 15 + 1] = 0.0;
			vertices[18*(z * 50 + x) + 15 + 2] = z_off + 1.0;

			normals[18*(z * 50 + x) + 0] = 0.0;
			normals[18*(z * 50 + x) + 1] = 1.0;
			normals[18*(z * 50 + x) + 2] = 0.0;

			normals[18*(z * 50 + x) + 3 + 0] = 0.0;
			normals[18*(z * 50 + x) + 3 + 1] = 1.0;
			normals[18*(z * 50 + x) + 3 + 2] = 0.0;

			normals[18*(z * 50 + x) + 6 + 0] = 0.0;
			normals[18*(z * 50 + x) + 6 + 1] = 1.0;
			normals[18*(z * 50 + x) + 6 + 2] = 0.0;

			normals[18*(z * 50 + x) + 9 + 0] = 0.0;
			normals[18*(z * 50 + x) + 9 + 1] = 1.0;
			normals[18*(z * 50 + x) + 9 + 2] = 0.0;

			normals[18*(z * 50 + x) + 12 + 0] = 0.0;
			normals[18*(z * 50 + x) + 12 + 1] = 1.0;
			normals[18*(z * 50 + x) + 12 + 2] = 0.0;

			normals[18*(z * 50 + x) + 15 + 0] = 0.0;
			normals[18*(z * 50 + x) + 15 + 1] = 1.0;
			normals[18*(z * 50 + x) + 15 + 2] = 0.0;
		}
	}

	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sea::~Sea() {
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &normalBufferObject);
}


void Sea::draw() {
	//Bind the arrays to the vao
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 50 * 50);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
