#ifndef PARTICLEEXPLOSIONNODE_H
#define PARTICLEEXPLOSIONNODE_H

#include "scenegraph.h"

struct  Particle{
	QVector3D pos;
	QVector3D velocity;
	float alpha;
	float rotate;
	float size;
	float time;
	float lifeTime;
};

class ParticleExplosionNode: public SceneGraph
{

private:
	GLuint vertexBufferObject;
	GLuint texCoordsBufferObject;
	GLuint alphaBufferObject;
	void drawParticles();
	void emitParticles();
	void randomizeParticles();

	int minWidth= 0.0;
	int maxWidth= 1.0;
	int minHeight= 0.0;
	int maxHeight= 1.0;
	int minDepth= 0.0;
	int maxDepth= 1.0;
	int minLifeTime=0.0;
	int maxLifeTime=11.0;
	int minSpeed=1;
	int maxSpeed=2;
	float emmiterLifetime=0.0;
	float emmiterMaxLifetime=5.0;
	QVector3D force= QVector3D(0, -2.81f, 0);

	GLuint sprite;

public:
	ParticleExplosionNode(QVector3D pos, Primitive *p, std::string name);

	~ParticleExplosionNode();
	void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);
	void exeObjBehaviour();
	void reactToCollision();
	BoundingBox* getBB();


	void emitParticle(Particle &particle);
	std::vector<Particle> particles;
	void updateParticles(float deltaTime);

	//Erzeugt ein Partikel und legt es auf die beigelegte Adresse

	void buildVertexBuffer();
	float lerp(float ,float ,float);
	QVector3D emitterPos;
	int nParticles;

};

#endif // PARTICLEEXPLOSIONNODE_H
