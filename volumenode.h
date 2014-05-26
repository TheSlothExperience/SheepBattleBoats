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

public:
    VolumeNode(Volume *p, std::string name);
    void drawBB(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);
};

#endif
