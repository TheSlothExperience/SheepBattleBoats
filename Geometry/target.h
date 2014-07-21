#ifndef TARGET_H
#define TARGET_H

#include <primitive.h>
#include<boundingbox.h>

class Target: public Primitive
{

private:
    GLuint indexBufferObject;
//    int count;
    BoundingBox *bb;
    int count;
    int numVerts;
    int parts;
    int idxPerPart;


public:
    Target();
    ~Target();
    bool intersect();
    void draw();
    BoundingBox *getBB();
};

#endif // TARGET_H
