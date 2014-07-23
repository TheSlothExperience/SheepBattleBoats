#define GL_GLEXT_PROTOTYPES

#include "projectile.h"
#include<limits>
#include<QtGui>

Projectile::Projectile(QVector3D pos)
{
//    this->shootingDir=dir;
    int tesselation = pow(2, 8) + 1;
    int rings = tesselation;
    int sectors = tesselation;

    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    float radius = 0.3;

    GLfloat *vertices = new GLfloat[rings * sectors * 3];
    GLfloat *normals = new GLfloat[rings * sectors * 3];
    GLshort *indices = new GLshort[rings * sectors * 4];
    float maxFloat=std::numeric_limits<float>::max();
    float minFloat=std::numeric_limits<float>::min();
    QVector3D min=QVector3D(maxFloat,maxFloat,maxFloat);
    QVector3D max=QVector3D(minFloat,minFloat,minFloat);

    count = rings * sectors * 4;
    for(int r = 0; r < rings; r++) {
        for(int s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
            int i = (r*sectors + s)*3;
            vertices[i] = x * radius;
            if(vertices[i]>max[0]) max[0]=vertices[i];
            if(vertices[i]<min[0]) min[0]=vertices[i];
            vertices[i + 1] = y * radius;
            if(vertices[i+1]>max[1]) max[1]=vertices[i+1];
            if(vertices[i+1]<min[1]) min[1]=vertices[i+1];
            vertices[i + 2] = z * radius;
            if(vertices[i+2]>max[2]) max[2]=vertices[i+2];
            if(vertices[i+2]<min[2]) min[2]=vertices[i+2];

            normals[i] = x;
            normals[i + 1] = y;
            normals[i + 2] = z;
        }
    }

    bb=new BoundingBox(pos,min,max);
    for(int r = 0; r < rings-1; r++) {
        for(int s = 0; s < sectors-1; s++) {
            int i = (r*(sectors-1) + s) * 4;
            indices[i+1] = r * sectors + s;
            indices[i+0] = r * sectors + (s+1);
            indices[i+3] = (r+1) * sectors + (s+1);
            indices[i+2] = (r+1) * sectors + s;
        }
    }

    //Send the buffers to OpenGL
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * (sectors*rings) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 3 * (sectors*rings) * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 4 * (sectors*rings) * sizeof(GLshort), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] vertices;
    delete[] normals;
    delete[] indices;
}

Projectile::~Projectile(){
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &normalBufferObject);
    glDeleteBuffers(1, &indexBufferObject);

}


void Projectile::draw(){
    // //Bind the arrays to the vao
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Draw the indexed vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glDrawElements(GL_QUADS, count, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


BoundingBox* Projectile::getBB(){
return this->bb;
}
