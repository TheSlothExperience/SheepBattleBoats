#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"
#include "scenegraph.h"


class LightNode : public SceneGraph
{
private:

	QVector4D lightColor;
	GLuint fbo;
	GLuint shadowMap;

public:
	LightNode(Primitive *p, std::string name);
	~LightNode();

	QVector3D getPosition();
	QVector4D getLightColor() {return lightColor;}
	void setLightColor(QVector4D lc) {lightColor = lc;}

	QMatrix4x4 lightView() {
		QMatrix4x4 view();
		float x = translation.x();
		float z = translation.z();
		QVector3D up;
		if(x != 0 || z != 0) {
			up = QVector3D::crossProduct(QVector3D(-z, 0, x), translation);
		} else {
			//Looking from the top
			up = QVector3D(0.0, 0.0, -1.0);
		}

		view.lookAt(translation, QVector3D(0, 0, 0), up);
		return view;
	}

	QMatrix4x4 lightPerspective() {
		QMatrix4x4 persp();
		persp.perspective(45, 1024.0 / 768.0, -0.1, 100);
		return persp;
	}
	void lightPass();
	GLuint shadowFBO() {return fbo;}
	GLuint shadowMap() {return shadowMap;}

	void changeColor(float r, float g, float b, float a);
};

#endif
