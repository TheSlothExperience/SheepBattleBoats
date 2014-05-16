#ifndef SCENE_H
#define SCENE_H

#include "scenegraph.h"

#include <stack>
#include <QMatrix4x4>

class Scene
{
private:
    SceneGraph *sceneGraph;

    
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;

    QVector4D lightPosition;
    std::stack<QMatrix4x4> modelViewMatrixStack;

public:
    Scene();
    Scene(GLuint mvLoc, GLuint normalLoc);
    void setLightLocation(GLuint lightPositionLocation);
    void draw(QMatrix4x4 cameraMatrix);
};

#endif //SCENE_H
