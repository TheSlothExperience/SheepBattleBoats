#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/gl.h>
#include <QOpenGLShader>

class Primitive
{
protected:
    GLuint vertexBufferObject;
    GLuint normalBufferObject;

public:
    virtual ~Primitive() = 0;
    virtual void draw() = 0;
	virtual void draw(QOpenGLShaderProgram *) {draw();}
};

inline Primitive::~Primitive() {}
#endif //PRIMITIVE_H
