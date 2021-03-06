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
	GLuint noiseTexture;
	GLfloat *noiseData;

	int seaWidth;
	int seaHeight;
	float periodicity;

public:
	SeaNode(Primitive *p, std::string name);
	~SeaNode();

	int getSeaWidth() {return seaWidth;}
	int getSeaHeight() {return seaHeight;}

	void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix);
	void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);
	void drawGeometry(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);
};

#endif
