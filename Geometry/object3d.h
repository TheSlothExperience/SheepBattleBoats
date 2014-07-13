#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>
#include <vector>
#include <QVector4D>
#include <QVector3D>
#include "primitive.h"

class Object3D : public Primitive
{
private:
    GLuint indexBufferObject;
    int count;
    GLfloat *vertices;
    GLfloat *normals;
public:
    Object3D();
    ~Object3D();
    void draw();
    GLfloat* getVertices(){return vertices;}
    GLfloat* getNormals(){return normals;}
    void setVertices(GLfloat* vert){this->vertices = vert;}
    void setNormals(GLfloat* norm){this->normals = norm;}
    void load(const char* filename, std::vector<QVector4D> &vertices, std::vector<QVector3D> &normals, std::vector<GLushort> &elements);
};

#endif // OBJECT3D_H
