#define GL_GLEXT_PROTOTYPES

#include "object3d.h"
#include "vector"

Object3D::Object3D(){

    int vertexCount = 8;
    vertices = new GLfloat[vertexCount];
    normals = new GLfloat[vertexCount];
    //Send the buffers to OpenGL
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertices) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertices) * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    delete[] vertices;
    delete[] normals;
}

void Object3D::draw(){
    // //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

Object3D::~Object3D(){

    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &normalBufferObject);

}

void Object3D::load(const char* filename, std::vector<QVector4D> &vertices, std::vector<QVector3D> &normals, std::vector<GLushort> &elements){


}
