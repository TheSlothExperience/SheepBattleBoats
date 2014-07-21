#ifndef PARALLELLIGHT_H
#define PARALLELLIGHT_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#include "primitive.h"
#include "lightnode.h"


class ParallelLight : public LightNode
{
public:
	ParallelLight(Primitive *p, std::string name);
	~ParallelLight();

	QVector3D getPosition();
	QVector4D getLightColor() {return lightColor;}
	void setLightColor(QVector4D lc) {lightColor = lc;}

	QMatrix4x4 lightView() {
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

	QMatrix4x4 perspectiveMatrix() {
		QMatrix4x4 persp = QMatrix4x4();
		persp.ortho(-5, 5, -5, 5, -100, 100);
		return persp;
	}
	void lightPass();
	GLuint shadowFBO() {return fbo;}
	GLuint shadowMoments() {return moments;}
	GLuint shadowMomentsTemp() {return momentsTemp;}
	GLuint getShadowMap() {return shadowMap;}

	int shadowWidth() {return m_shadowWidth;}
	int shadowHeight() {return m_shadowHeight;}

	void changeColor(float r, float g, float b, float a);

};

#endif
