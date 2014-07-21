#ifndef PARTICLEEXPLOSIONNODE_H
#define PARTICLEEXPLOSIONNODE_H
#include "scenegraph.h"

class ParticleExplosionNode: public SceneGraph
{

private:
    GLuint vertexBufferObject;
    GLuint texCoordsBufferObject;
    GLuint alphaBufferObject;
public:
    ParticleExplosionNode(QVector3D pos, Primitive *p, std::string name);

    ~ParticleExplosionNode();
    void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);
    void exeObjBehaviour();
    void reactToCollision();
    BoundingBox* getBB();

    struct  Particle{
      QVector3D pos;
      QVector3D velocity;
//      QVector4D color;
      float alpha;
      float rotate;
      float size;
      float time;
      float lifeTime;
    };

    std::vector<Particle> particles;
    void updateParticles();

    //Erzeugt ein Partikel und legt es auf die beigelegte Adresse
    void emitParticle(Particle &particle);
    void buildVertexBuffer();
    float lerp(float ,float ,float);
    QVector3D emitterPos;

};


#endif // PARTICLEEXPLOSIONNODE_H
