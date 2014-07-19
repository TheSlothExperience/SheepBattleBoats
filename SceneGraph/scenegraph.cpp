#define GL_GLEXT_PROTOTYPES

#include "scenegraph.h"
#include "glwidgetcontext.h"

#include <algorithm>
#include <functional>
#include <QMatrix4x4>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

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
	color[0] = 0.8;
	color[1] = 0.3;
	color[2] = 0.0;
	color[3] = 1.0;
}

SceneGraph::SceneGraph(Primitive *p, SceneGraph *parent) {
	this->primitive = p;
	this->leaf = true;
	name = "SceneGraph w. Primitive";
	this->parentNode = parent;
	color[0] = 0.8;
	color[1] = 0.3;
	color[2] = 0.0;
	color[3] = 1.0;
}

SceneGraph::SceneGraph(Primitive *p, std::string name) {
	this->primitive = p;
	this->leaf = true;
	this->name = name;
	color[0] = 0.8;
	color[1] = 0.3;
	color[2] = 0.0;
	color[3] = 1.0;
}

SceneGraph::~SceneGraph() {
	delete primitive;
	for(unsigned int i = 0; i < children.size(); i++) {
		delete children.at(i);
	}
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

bool SceneGraph::setData(int column, const QVariant &value) {
	if(column != 0) {
		return false;
	} else {
		this->name = value.toString().toUtf8().constData();
		return true;
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

bool SceneGraph::removeChildren(int position, int count) {
	if (position < 0 || (unsigned int)(position + count) > children.size()) {
		return false;
	}

	for (int row = 0; row < count; ++row) {
		SceneGraph *toDelete = children.at(position + row);
		delete toDelete;
		children.erase(children.begin() + position + row);
	}
	return true;
}

bool SceneGraph::insertChildren(int position, int count, int) {
	if (position < 0 || (unsigned int) (position + count) > children.size()) {
		return false;
	}

	for (int row = 0; row < count; ++row) {
		SceneGraph *s = new SceneGraph(false, this);
		children.insert(children.begin() + position + row, s);
	}

	return true;
}

SceneGraph* SceneGraph::add(Primitive *p) {
	if(leaf) {
		D(std::cout << "Trying to add a subtree into a leaf!!" << std::cout;)
			return 0;
	} else {
		SceneGraph *s = new SceneGraph(p, this);
		this->children.push_back(s);
		return s;
	}
}

SceneGraph* SceneGraph::add(SceneGraph *s) {
	if(leaf) {
		if(parentNode) {
			return parentNode->add(s);
		} else {
			return 0;
		}
	} else {
		s->setParent(this);
		this->children.push_back(s);
		return s;
	}
}

void SceneGraph::setParent(SceneGraph *s) {
	this->parentNode = s;
}

void SceneGraph::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix) {
	draw(MVStack, cameraMatrix, projectionMatrix, Shaders::shaderProgram);
}

void SceneGraph::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
	Shaders::bind(shader);
	MVStack.push(MVStack.top());

	MVStack.top().translate(this->translation);

	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;

	//If the node is a leaf, draw its contents
	if(leaf) {
		glUniformMatrix4fv(shader->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
		glUniformMatrix4fv(shader->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());
		glUniformMatrix4fv(shader->uniformLocation("normalMatrix"), 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
		int r = (id & 0x000000FF) >>  0;
		int g = (id & 0x0000FF00) >>  8;
		int b = (id & 0x00FF0000) >> 16;
		glUniform4f(shader->uniformLocation("id"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

		glUniform4fv(shader->uniformLocation("color"), 1, color);

		this->primitive->draw();
	} else {
		//Else, recurse into its children
		std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->draw(MVStack, cameraMatrix, projectionMatrix, shader);});
	}

	MVStack.pop();
	Shaders::release(shader);
}

void SceneGraph::drawGeometry(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
	Shaders::bind(shader);
	MVStack.push(MVStack.top());

	MVStack.top().translate(this->translation);

	//Convert the quat to a matrix, may be a performance leak.
	QMatrix4x4 tempRot;
	tempRot.rotate(this->rotation.normalized());
	MVStack.top() *= tempRot;

	//If the node is a leaf, draw its contents
	if(leaf) {
		glUniformMatrix4fv(shader->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
		glUniformMatrix4fv(shader->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());
		glUniformMatrix4fv(shader->uniformLocation("normalMatrix"), 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
		int r = (id & 0x000000FF) >>  0;
		int g = (id & 0x0000FF00) >>  8;
		int b = (id & 0x00FF0000) >> 16;
		glUniform4f(shader->uniformLocation("id"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

		glUniform4fv(shader->uniformLocation("color"), 1, color);

		this->primitive->draw();
	} else {
		//Else, recurse into its children
		std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->drawGeometry(MVStack, cameraMatrix, projectionMatrix, shader);});
	}

	MVStack.pop();
	Shaders::release(shader);
}

void SceneGraph::setName(std::string name) {
	this->name = name;
}

void SceneGraph::translate(double x, double y, double z) {
	translation += QVector3D(x, y, z);
}

void SceneGraph::rotate(QQuaternion q) {
	rotation = q * rotation;
}
