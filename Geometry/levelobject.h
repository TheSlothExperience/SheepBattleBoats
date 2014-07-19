#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H
#include <primitive.h>
#include<boundingbox.h>

class LevelObject: public Primitive
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
    LevelObject();
    ~LevelObject();
    bool intersect();
    void draw();
    BoundingBox *getBB();


};

#endif // LEVELOBJECT_H
