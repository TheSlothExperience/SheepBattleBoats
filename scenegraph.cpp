#define GL_GLEXT_PROTOTYPES

#include "scenegraph.h"

#include <algorithm>
#include <functional>
#include <QMatrix4x4>
#include <GL/gl.h>

#ifdef DEBUG 
#define D(x) x
#else 
#define D(x)
#endif

SceneGraph::SceneGraph(bool isleaf) {
    this->leaf = isleaf;
    primitive = NULL;
}

SceneGraph::SceneGraph(Primitive *p) {
    this->primitive = p;
    this->leaf = true;
}


void SceneGraph::add(Primitive *p) {
    if(leaf) {
	D(std::cout << "Trying to add a subtree into a leaf!!" << std::cout;)
    } else {
	this->children.push_back(SceneGraph(p));    
    }
}

void SceneGraph::add(SceneGraph &s) {
    if(leaf) {
	D(std::cout << "Trying to add a subtree into a leaf!!" << std::cout;)
    } else {
	this->children.push_back(s);    
    }
}

void SceneGraph::draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc) {
    
    MVStack.push(MVStack.top());
    
    MVStack.top().translate(this->translation);
    
    //Convert the quat to a matrix, may be a performance leak.
    QMatrix4x4 tempRot;
    tempRot.rotate(this->rotation.normalized());
    MVStack.top() *= tempRot;
    
    if(leaf) {
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, MVStack.top().constData());
	glUniformMatrix4fv(normalLoc, 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
	this->primitive->draw();
    } else {
	std::for_each(children.begin(), children.end(), [&MVStack, mvLoc, normalLoc](SceneGraph &s){s.draw(MVStack, mvLoc, normalLoc);});
    }

    MVStack.pop();
}
