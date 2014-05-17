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

SceneGraph::SceneGraph(bool isleaf, SceneGraph *parent) {
    this->leaf = isleaf;
    primitive = NULL;
    name = "SceneGraph";
    this->parentNode = parent;
}

SceneGraph::SceneGraph(Primitive *p, SceneGraph *parent) {
    this->primitive = p;
    this->leaf = true;
    name = "SceneGraph w. Primitive";
    this->parentNode = parent;
}


SceneGraph* SceneGraph::parent() {
    return parentNode;
}

SceneGraph* SceneGraph::child(int row) {
    return children.at(row);
}

int SceneGraph::childCount() {
    return children.size();
}
int SceneGraph::columnCount() {
    return 1; //JUST THE NAME OF THE NODE
}
QVariant SceneGraph::data(int column) {
    if(column == 0) {
	return QVariant(name.c_str());
    } else {
	return QVariant();
    }
}

int SceneGraph::row() {
    if(parentNode) {
	auto pchildren = &(parentNode->children);
	return std::find(pchildren->begin(), pchildren->end(), this) - pchildren->begin();
    } else {
	return 0;
    }
}

void SceneGraph::add(Primitive *p) {
    if(leaf) {
	D(std::cout << "Trying to add a subtree into a leaf!!" << std::cout;)
    } else {
	this->children.push_back(new SceneGraph(p, this));    
    }
}

void SceneGraph::add(SceneGraph *s) {
    if(leaf) {
	D(std::cout << "Trying to add a subtree into a leaf!!" << std::cout;)
    } else {
	s->setParent(this);
	this->children.push_back(s);    
    }
}

void SceneGraph::setParent(SceneGraph *s) {
    this->parentNode = s;
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
	std::for_each(children.begin(), children.end(), [&MVStack, mvLoc, normalLoc](SceneGraph *s){s->draw(MVStack, mvLoc, normalLoc);});
    }

    MVStack.pop();
}
