#define GL_GLEXT_PROTOTYPES

#include "cube.h"

#include <iostream>

Cube::Cube() {
     
    static const GLfloat vertices[] = {
     	-0.5f,-0.5f,-0.5f, // triangle 1 : begin
     	-0.5f,-0.5f, 0.5f,
     	-0.5f, 0.5f, 0.5f, // triangle 1 : end
     	0.5f, 0.5f,-0.5f, // triangle 2 : begin
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f,-0.5f, // triangle 2 : end
     	0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f, 0.5f,
     	-0.5f,-0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	-0.5f,-0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f,-0.5f,
     	0.5f,-0.5f,-0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	0.5f, 0.5f,-0.5f,
     	-0.5f, 0.5f,-0.5f,
     	0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f,-0.5f,
     	-0.5f, 0.5f, 0.5f,
     	0.5f, 0.5f, 0.5f,
     	-0.5f, 0.5f, 0.5f,
     	0.5f,-0.5f, 0.5f
    };
    
    
    static const GLfloat normals[] = {
     	-1.0f,-0.0f,-0.0f, // left
     	-1.0f,-0.0f, 0.0f,
     	-1.0f, 0.0f, 0.0f, 
     	0.0f, 0.0f,-1.0f, // back
     	-0.0f,-0.0f,-1.0f,
     	-0.0f, 0.0f,-1.0f,
     	0.0f,-1.0f, 0.0f, // bottom
     	-0.0f,-1.0f,-0.0f,
     	0.0f,-1.0f,-0.0f,
     	0.0f, 0.0f,-1.0f, // back
     	0.0f,-0.0f,-1.0f,
     	-0.0f,-0.0f,-1.0f,
     	-1.0f,-0.0f,-0.0f,// left
     	-1.0f, 0.0f, 0.0f,
     	-1.0f, 0.0f,-0.0f,
     	0.0f,-1.0f, 0.0f, // bottom
     	-0.0f,-1.0f, 0.0f,
     	-0.0f,-1.0f,-0.0f,
     	-0.0f, 0.0f, 1.0f, // front 
     	-0.0f,-0.0f, 1.0f,
     	0.0f,-0.0f, 1.0f,
     	1.0f, 0.0f, 0.0f,  //right
     	1.0f,-0.0f,-0.0f,
     	1.0f, 0.0f,-0.0f,
     	1.0f,-0.0f,-0.0f, // right
     	1.0f, 0.0f, 0.0f,
     	1.0f,-0.0f, 0.0f,
     	0.0f, 1.0f, 0.0f, // top
     	0.0f, 1.0f,-0.0f,
     	-0.0f, 1.0f,-0.0f,
     	0.0f, 1.0f, 0.0f, // top
     	-0.0f, 1.0f,-0.0f,
     	-0.0f, 1.0f, 0.0f,
     	0.0f, 0.0f, 1.0f, // front
     	-0.0f, 0.0f, 1.0f,
     	0.0f, 0.0f, 1.0f
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

Cube::~Cube() {
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &normalBufferObject);
}

void Cube::draw() {
    //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3*12);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
