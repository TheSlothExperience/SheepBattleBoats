#define GL_GLEXT_PROTOTYPES

#include "scene.h"
#include "primitive.h"
#include "cube.h"

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include <iostream>

Scene::Scene() {
    this->modelViewMatrixStack.push(QMatrix4x4());
}
Scene::Scene(GLuint mvLoc, GLuint normalLoc) {
    this->modelViewMatLocation = mvLoc;
    this->normalMatLocation = normalLoc;
    this->modelViewMatrixStack.push(QMatrix4x4());

    Cube *cube = new Cube();
    this->sceneGraph.add(cube);
    lightPosition = QVector4D(0.5, 0.0, 2.0, 1.0);
}

void Scene::setLightLocation(GLuint lightPositionLocation) {
    this->lightPositionLocation = lightPositionLocation;
}

void Scene::draw(QMatrix4x4 cameraMatrix) {
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    modelViewMatrixStack.top() *= cameraMatrix;
    
    QVector4D lightDir = cameraMatrix * lightPosition;
    GLfloat lightDirArray[3] = {lightDir.x(), lightDir.y(), lightDir.z()};
    glUniform3fv(lightPositionLocation, 1, lightDirArray);
    
    this->sceneGraph.draw(modelViewMatrixStack, modelViewMatLocation, normalMatLocation);
    modelViewMatrixStack.pop();
}
