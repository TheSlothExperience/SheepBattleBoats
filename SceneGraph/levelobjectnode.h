#ifndef LEVELOBJECTNODE_H
#define LEVELOBJECTNODE_H

#include<scenegraph.h>
#include<levelobject.h>
#include<boundingbox.h>

class LevelObjectNode :public SceneGraph
{
private :
    LevelObject* lvlObj;
public:
    LevelObjectNode(LevelObject *p, std::string name);
    ~LevelObjectNode();
    void draw();
    void testCollisions(LevelObjectNode);
    BoundingBox *getBB();

    LevelObject getLvlObj();
};

#endif // LEVELOBJECTNODE_H
