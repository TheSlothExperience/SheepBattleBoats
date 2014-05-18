#define GL_GLEXT_PROTOTYPES

#include "cylinder.h"

#include <iostream>
#include <math.h>

Cylinder::Cylinder(int tesselationLevel) {
    
    const double PI = 3.141592653589793238463;
    int tesselation = pow(2, tesselationLevel) + 1; //at least 3

    //Number of VERTICES per triangle
    int topVertices = tesselation + 1; //plus center
    int topTriangles = tesselation;
    
    int bodyVertices = 2*tesselation;
    int bodyTriangles = 2*tesselation;
    
    int bottomVertices = tesselation + 1;
    int bottomTriangles = tesselation;

    int totalVertices = topVertices + bodyVertices + bottomVertices;
    int totalTriangles = topTriangles + bodyTriangles + bottomTriangles;
    
    GLfloat *vertices = new GLfloat[3 * totalVertices];
    GLfloat *normals = new GLfloat[3 * totalVertices];
    GLshort *indices = new GLshort[3 * totalTriangles];
    //Total number of vertices to be drawn
    count = 3 * totalTriangles;

    int offsetV, offsetI = 0;
    
    //Top circle
    //Center top vertex
    vertices[0] = 0.0f;
    vertices[1] = 0.5f;
    vertices[2] = 0.0f;
	    
    normals[0] = 0.0f;
    normals[1] = 1.0f;
    normals[2] = 0.0f;
    for(int i = 3; i < topVertices * 3; i = i + 3) {
    	double angle = 2 * PI * (((double) i /(double) 3) - 1) / (double)tesselation;
    	vertices[i] = 0.5f * sin(angle);
    	vertices[i+1] = 0.5f;
    	vertices[i+2] = 0.5f * cos(angle);
	    
    	normals[i] = 0.0f;
    	normals[i+1] = 1.0f;
    	normals[i+2] = 0.0f;
    }
    
    for(int i = 0; i < 3*topTriangles; i = i + 3) {
    	indices[i] = 0;
    	indices[i + 1] = (i / 3) + 1;
    	indices[i + 2] = (((i / 3) + 1 ) % tesselation) + 1;
    }
    
    //Body
    offsetV = topVertices;
    double norm = sqrt(2.0);
    for(int i = 0; i < bodyVertices * 3; i = i + 6) {
    	double angle = 2 * PI * (((double) i /(double) 6) - 1) / (double)tesselation;
    	vertices[3 * offsetV + i] = 0.5f * sin(angle);
    	vertices[3 * offsetV + i+1] = 0.5f;
    	vertices[3 * offsetV + i+2] = 0.5f * cos(angle);
	    
    	normals[3 * offsetV + i] = norm * sin(angle);
    	normals[3 * offsetV + i+1] = 0.0f;
    	normals[3 * offsetV + i+2] = norm * cos(angle);
	
    	vertices[3 * offsetV + i+3] = 0.5f * sin(angle);
    	vertices[3 * offsetV + i+4] = -0.5f;
    	vertices[3 * offsetV + i+5] = 0.5f * cos(angle);
	    
    	normals[3 * offsetV + i+3] = norm * sin(angle);
    	normals[3 * offsetV + i+4] = 0.0f;
    	normals[3 * offsetV + i+5] = norm * cos(angle);
    }
    

    offsetI = 3 * topTriangles;
    for(int i = 0; i < 3 * bodyTriangles; i = i + 6) {
    	indices[offsetI + i] = offsetV + 2*(i/6);
    	indices[offsetI + i + 1] = offsetV + 2*(i/6) + 1;
    	indices[offsetI + i + 2] = offsetV + ((2*(i/6) + 2) % bodyTriangles);
	
    	indices[offsetI + i + 3] = offsetV + 2*(i/6) + 1;
    	indices[offsetI + i + 4] = offsetV + ((2*(i/6) + 3) % bodyTriangles);
    	indices[offsetI + i + 5] = offsetV + ((2*(i/6) + 2) % bodyTriangles);
    }
    
    //Bottom circle
    offsetV = topVertices + bodyVertices;
    //Center bottom vertex
    vertices[3 * offsetV + 0] = 0.0f;
    vertices[3 * offsetV + 1] = -0.5f;
    vertices[3 * offsetV + 2] = 0.0f;
	    
    normals[3 * offsetV + 0] = 0.0f;
    normals[3 * offsetV + 1] = -1.0f;
    normals[3 * offsetV + 2] = 0.0f;
    for(int i = 3; i < 3 * bottomVertices; i = i + 3) {
    	double angle = 2 * PI * (((double) i /(double) 3) - 1) / (double)tesselation;
    	vertices[3 * offsetV + i] = 0.5f * sin(angle);
    	vertices[3 * offsetV + i+1] = -0.5f;
    	vertices[3 * offsetV + i+2] = 0.5f * cos(angle);
	    
    	normals[3 * offsetV + i] = 0.0f;
    	normals[3 * offsetV + i+1] = -1.0f;
    	normals[3 * offsetV + i+2] = 0.0f;
    }

    offsetI = 3 * (topTriangles + bodyTriangles);
    for(int i = 0; i < 3*bottomTriangles; i = i + 3) {
    	indices[offsetI + i] = offsetV + 0;
	//Change the polarity of the neutron beam!
    	indices[offsetI + i + 2] = offsetV + (i / 3) + 1;
    	indices[offsetI + i + 1] = offsetV + (((i / 3) + 1 ) % tesselation) + 1;
    }

    //Send the buffers to OpenGL
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * totalVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * totalVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * totalTriangles * sizeof(GLshort), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] vertices;
    delete[] normals;
    delete[] indices;
}

void Cylinder::draw() {
    // //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Draw the indexed vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
}
