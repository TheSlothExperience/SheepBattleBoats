#ifndef VOLUMENODE_H
#define VOLUMENODE_H

#include <vector>
#include "volume.h"
#include "scenegraph.h"
#include "primitive.h"

class VolumeNode : public SceneGraph
{
private:
    Volume* volume;
    GLuint tex3DLocation;

    unsigned char transferFunction[256][4];
public:
    VolumeNode(Volume *p, std::string name);
    void drawBB(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);

    void loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned char* raw);
    GLuint getTexLocation() {return tex3DLocation;}
};

#endif
