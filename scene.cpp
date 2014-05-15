#include "scene.h"
#include "primitive.h"
#include "cube.h"

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>

Scene::Scene() {
    this->modelViewMatrixStack.push(QMatrix4x4());
}
Scene::Scene(GLuint mvLoc, GLuint normalLoc) {
    this->modelViewMatLocation = mvLoc;
    this->normalMatLocation = normalLoc;
    this->modelViewMatrixStack.push(QMatrix4x4());

    Cube *cube = new Cube();
    this->sceneGraph.add(cube);
    
}

void Scene::draw(QMatrix4x4 cameraMatrix) {
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    modelViewMatrixStack.top() *= cameraMatrix;
    this->sceneGraph.draw(modelViewMatrixStack, modelViewMatLocation, normalMatLocation);
    modelViewMatrixStack.pop();
}
