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
protected:

	QVector4D lightColor;
	GLuint fbo;
	GLuint shadowMap;
	GLuint moments;
	GLuint momentsTemp;
	GLuint depthBuffer;
	int m_shadowWidth;
	int m_shadowHeight;

public:
	LightNode(Primitive *p, std::string name);
	virtual ~LightNode();

	virtual QVector3D getPosition();
	virtual QVector4D getLightColor() {return lightColor;}
	virtual void setLightColor(QVector4D lc) {lightColor = lc;}

	virtual QMatrix4x4 lightView() {
		QMatrix4x4 view = QMatrix4x4();
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

	virtual QMatrix4x4 perspectiveMatrix() {
		QMatrix4x4 persp = QMatrix4x4();
		//float aspect = 1024.0 / 768.0;
		//persp.perspective(45, 1024.0 / 768.0, 0.1, 100);
		//persp.ortho(-aspect, aspect, -1, 1, -100, 100);
		persp.ortho(-5, 5, -5, 5, -100, 100);
		return persp;
	}
	virtual void lightPass();
	virtual GLuint shadowFBO() {return fbo;}
	virtual GLuint shadowMoments() {return moments;}
	virtual GLuint shadowMomentsTemp() {return momentsTemp;}
	virtual GLuint getShadowMap() {return shadowMap;}

	virtual int shadowWidth() {return m_shadowWidth;}
	virtual int shadowHeight() {return m_shadowHeight;}

	void changeColor(float r, float g, float b, float a);

};

#endif
