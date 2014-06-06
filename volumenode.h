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
    bool mip = false;
    bool iso = false;
    int isoValue = 100;
    
    unsigned char transferFunction[256][4];
public:
    VolumeNode(Volume *p, std::string name);
    ~VolumeNode();
    void drawBB(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc);

    void loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned char* raw);
    void loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned short* raw);
    GLuint getTexLocation() {return tex3DLocation;}
    GLuint getTFLocation() {return tfLocation;}

    bool getMIP() {return mip;}
    void setMIP(bool value) {mip = value;}

    bool showIso() {return iso;}
    void setIso(bool show) {iso = show;}

    int getIsoValue() {return isoValue;}
    void setIsoValue(int newVal) {isoValue = newVal;}
    
    void changeTF(unsigned char tf[][4]);
};

#endif
