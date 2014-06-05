#define GL_GLEXT_PROTOTYPES

#include "volumenode.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

VolumeNode::VolumeNode(Volume *p, std::string name)
    : SceneGraph(p, name)
{
    this->volume = p;
    glGenTextures(1, &tex3DLocation);
    glBindTexture(GL_TEXTURE_3D, tex3DLocation);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_3D, 0);

    //Init transferfunction as a diagonal
    for(int i = 0; i < 256; i++) {
	transferFunction[i][0] = i;
	transferFunction[i][1] = i;
	transferFunction[i][2] = i;
	transferFunction[i][3] = i;
    }
    glGenTextures(1, &tfLocation);
    glBindTexture(GL_TEXTURE_1D, tfLocation);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, 
             GL_UNSIGNED_BYTE, (void*)transferFunction);
    glBindTexture(GL_TEXTURE_1D, 0);
}


void VolumeNode::changeTF(unsigned char tf[][4]) {
    for(int i = 0; i < 256; i++) {
	transferFunction[i][0] = tf[i][0];
	transferFunction[i][1] = tf[i][1];
	transferFunction[i][2] = tf[i][2];
	transferFunction[i][3] = tf[i][3];
    }
    glBindTexture(GL_TEXTURE_1D, tfLocation);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, 
             GL_UNSIGNED_BYTE, (void*)transferFunction);
    glBindTexture(GL_TEXTURE_1D, 0);
}

void VolumeNode::loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned char* raw) {
    //Create a new texture with the right size and data
    glBindTexture(GL_TEXTURE_3D, tex3DLocation);
    //MAKE SURE THE ROWS ARE ALIGNED!!!
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, x, y, z, 0, GL_RED, 
             GL_UNSIGNED_BYTE, (void*)raw);
    glBindTexture(GL_TEXTURE_3D, 0);
    
    delete this->volume;
    float nx = (float) x * ax;
    float ny = (float) y * ay;
    float nz = (float) z * az;
    //Normalize the aspects
    float m = std::max(nx, std::max(ny, nz));
    nx /= m; ny /= m; nz /= m;
    this->volume = new Volume(nx, ny, nz);
}

void VolumeNode::loadTexture(int x, int y, int z, float ax, float ay, float az, unsigned short* raw) {
    //Create a new texture with the right size and data
    glBindTexture(GL_TEXTURE_3D, tex3DLocation);
    //MAKE SURE THE ROWS ARE ALIGNED!!!
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glPixelStorei(GL_PACK_ALIGNMENT, 2);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, x, y, z, 0, GL_RED, 
             GL_UNSIGNED_SHORT, (void*)raw);
    glBindTexture(GL_TEXTURE_3D, 0);
    
    delete this->volume;
    float nx = (float) x * ax;
    float ny = (float) y * ay;
    float nz = (float) z * az;
    //Normalize the aspects
    float m = std::max(nx, std::max(ny, nz));
    nx /= m; ny /= m; nz /= m;
    this->volume = new Volume(nx, ny, nz);
}

void VolumeNode::drawBB(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc) {
    
    MVStack.push(MVStack.top());  
    MVStack.top().translate(this->translation);
    
    //Convert the quat to a matrix, may be a performance leak.
    QMatrix4x4 tempRot;
    tempRot.rotate(this->rotation.normalized());
    MVStack.top() *= tempRot;
    
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, MVStack.top().constData());
    
    this->volume->drawBB();
    
    MVStack.pop();
}
