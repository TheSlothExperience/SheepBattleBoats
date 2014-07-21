#ifndef PROJECTILE_H
#define PROJECTILE_H
#include<primitive.h>

class Projectile:public Primitive
{
private:
    GLuint indexBufferObject;
    int count;
public:
    Projectile(QVector3D pos);
    ~Projectile();
    void draw();
    BoundingBox *getBB();
};

#endif // PROJECTILE_H
