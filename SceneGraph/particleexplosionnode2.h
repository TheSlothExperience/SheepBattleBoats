#ifndef PARTICLEEXPLOSIONNODE2_H
#define PARTICLEEXPLOSIONNODE2_H
#include "scenegraph.h"


//struct  Particle{
//  QVector3D pos;
//  QVector3D velocity;
////      QVector4D color;
//  float alpha;
//  float rotate;
//  float size;
//  float time;
//  float lifeTime;
//};

class ParticleExplosionNode2: public SceneGraph
{

private:
    GLuint vertexBufferObject;
    GLuint texCoordsBufferObject;
    GLuint alphaBufferObject;
    void drawParticles();
    void emitParticles();
    void randomizeParticles();

	QQuaternion lastCameraRotation;

    int minWidth= 0.0;
    int maxWidth= 1.0;
    int minHeight= 0.0;
    int maxHeight= 1.0;
    int minDepth= 0.0;
    int maxDepth= 1.0;
    int minLifeTime=5;
    int maxLifeTime=11.0;
    int minSpeed=1;
    int maxSpeed=1;
    float emmiterLifetime=0.0;
    float emmiterMaxLifetime=7.0;
    QVector3D force= QVector3D(0, 0.81f, 0);

    GLuint sprite;

public:
    ParticleExplosionNode2(QVector3D pos, Primitive *p, std::string name);

    ~ParticleExplosionNode2();
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



#endif // PARTICLEEXPLOSIONNODE2_H
