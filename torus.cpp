#define GL_GLEXT_PROTOTYPES

#include "torus.h"

#include <iostream>
#include <math.h>

Torus::Torus(int tesselationLevel) {
    
    const double PI = 3.141592653589793238463;
    int tesselation = pow(2, tesselationLevel) + 1; //at least 3

    numVerts = tesselation * tesselation;
    parts = tesselation;
    idxPerPart = (tesselation + 1) * 2;
    
    GLfloat *vertices = new GLfloat[3 * numVerts];
    GLfloat *normals = new GLfloat[3 * numVerts];
    GLshort *indices = new GLshort[(tesselation + 1) * 2 * tesselation];

    float oradius = 0.5f;
    float iradius = 0.25f;
    
    for(int j = 0; j < tesselation; j++) {
	for(int i = 0; i < tesselation; i++) {
	    int offset = 3 * (j * tesselation + i);
	    double psi = 2 * PI * (double) j / (double)tesselation;
	    double theta = 2 * PI * (double) i / -(double)tesselation;
    	
	    vertices[offset+0] = cos(psi) * (oradius + cos(theta) * iradius);
	    vertices[offset+1] = sin(psi) * (oradius + cos(theta) * iradius);
	    vertices[offset+2] = sin(theta) * iradius;

	    normals[offset+0] = cos(psi) * cos(theta);
	    normals[offset+1] = sin(psi) * cos(theta);
	    normals[offset+2] = sin(theta);
	}
    }

    for(int i = 0, idx = 0; i <tesselation; i++) {
	int ioff = 1;
	if(i == tesselation - 1) ioff = -i;
	for(int j = 0; j < tesselation; j++, idx+=2) {
	    int offset = j * tesselation + i;
	    indices[idx] = offset;
	    indices[idx+1] = offset + ioff;
	}
	indices[idx] = i;
	indices[idx+1] = i + ioff;
	idx += 2;
    }

    //Send the buffers to OpenGL
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVerts * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVerts * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, (tesselation + 1) * 2 * tesselation * sizeof(GLshort), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] vertices;
    delete[] normals;
    delete[] indices;
}

Torus::~Torus() {
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &normalBufferObject);
    glDeleteBuffers(1, &indexBufferObject);
}

void Torus::draw() {
    // //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Draw the indexed vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    for(int i = 0; i < parts; i++) {
	glDrawElements(GL_TRIANGLE_STRIP, idxPerPart, GL_UNSIGNED_SHORT, (GLvoid*) (sizeof(GLshort)*i*idxPerPart));
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
