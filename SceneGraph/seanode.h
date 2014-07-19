#ifndef SEANODE_H
#define SEANODE_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"
#include "scenegraph.h"

class SeaNode : public SceneGraph
{
private:

public:
	SeaNode(Primitive *p, std::string name);
	~SeaNode();

	void draw(std::stack<QMatrix4x4> &MVStack);
	void draw(std::stack<QMatrix4x4> &MVStack, QOpenGLShaderProgram *shader);
};

#endif
