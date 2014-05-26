#define GL_GLEXT_PROTOTYPES

#include "volumenode.h"
#include <GL/gl.h>
#include <GL/glext.h>

VolumeNode::VolumeNode(Volume *p, std::string name)
    : SceneGraph(p, name)
{
    this->volume = p;
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
