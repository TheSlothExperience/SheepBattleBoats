#define GL_GLEXT_PROTOTYPES
#include "volume.h"

#include <iostream>


Volume::Volume() {
     
    static GLfloat vertices[] = {
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
    static GLfloat texcoords[] = {
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
    
    static GLfloat normals[] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &texcoordsBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
}



Volume::Volume(float ax, float ay, float az) {
    float x = ax/2.0;
    float y = ay/2.0;
    float z = az/2.0;
    
    GLfloat vertices[108] = {
     	-x,-y,-z, // triangle 1 : begin
     	-x,-y, z,
     	-x, y, z, // triangle 1 : end
     	x, y,-z, // triangle 2 : begin
     	-x,-y,-z,
     	-x, y,-z, // triangle 2 : end
     	x,-y, z,
     	-x,-y,-z,
     	x,-y,-z,
     	x, y,-z,
     	x,-y,-z,
     	-x,-y,-z,
     	-x,-y,-z,
     	-x, y, z,
     	-x, y,-z,
     	x,-y, z,
     	-x,-y, z,
     	-x,-y,-z,
     	-x, y, z,
     	-x,-y, z,
     	x,-y, z,
     	x, y, z,
     	x,-y,-z,
     	x, y,-z,
     	x,-y,-z,
     	x, y, z,
     	x,-y, z,
     	x, y, z,
     	x, y,-z,
     	-x, y,-z,
     	x, y, z,
     	-x, y,-z,
     	-x, y, z,
     	x, y, z,
     	-x, y, z,
     	x,-y, z
    };
    static GLfloat texcoords[] = {
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
    
    static GLfloat normals[] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &texcoordsBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
}

Volume::~Volume() {
   glDeleteBuffers(1, &vertexBufferObject);
   glDeleteBuffers(1, &normalBufferObject);
   glDeleteBuffers(1, &texcoordsBufferObject);
}

void Volume::draw() {
    
}

void Volume::drawVolume() {
    
}

void Volume::drawBB() {
    //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBufferObject);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3*12);
}
