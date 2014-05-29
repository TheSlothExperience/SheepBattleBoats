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
    GLuint tfLocation;

    unsigned char transferFunction[256][4];
public:
    VolumeNode(Volume *p, std::string name);
    void drawBB(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);

    void loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned char* raw);
    void loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned short* raw);
    GLuint getTexLocation() {return tex3DLocation;}
    GLuint getTFLocation() {return tfLocation;}

    void changeTF(unsigned char tf[][4]);
};

#endif
