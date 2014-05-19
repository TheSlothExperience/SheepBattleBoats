#define GL_GLEXT_PROTOTYPES

#include "scene.h"
#include "primitive.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"
#include "sphere.h"
#include "torus.h"

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include <iostream>

Scene::Scene(QObject *parent)
    : QAbstractItemModel(parent)
{
    this->modelViewMatrixStack.push(QMatrix4x4());
}
Scene::Scene(GLuint mvLoc, GLuint normalLoc, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->modelViewMatLocation = mvLoc;
    this->normalMatLocation = normalLoc;
    this->modelViewMatrixStack.push(QMatrix4x4());

    this->rootDummy = new SceneGraph();
    this->rootNode = new SceneGraph();
    this->rootDummy->add(rootNode);
    
    lightPosition = QVector4D(0.5, 0.0, 2.0, 1.0);
}
 

QModelIndex Scene::index(int row, int column, const QModelIndex &parent) const {
    if(!hasIndex(row, column, parent)) {
	return QModelIndex();
    }

    SceneGraph *parentNode;

    if(!parent.isValid()) {
	parentNode = rootDummy;
    } else {
	parentNode = static_cast<SceneGraph*>(parent.internalPointer());
    }

    SceneGraph *childNode = parentNode->child(row);
    if(childNode) {
	return createIndex(row, column, childNode);
    } else {
	return QModelIndex();
    }
}

QModelIndex Scene::parent(const QModelIndex &index) const {
    if(!index.isValid()) {
	return QModelIndex();
    }

    SceneGraph *childNode = static_cast<SceneGraph*>(index.internalPointer());
    SceneGraph *parentNode = childNode->parent();

    if(parentNode == rootDummy) {
	return QModelIndex();
    } else {
	return createIndex(parentNode->row(), 0, parentNode);
    }
}

int Scene::rowCount(const QModelIndex &parent) const {
    SceneGraph *parentNode;
    if(parent.column() > 0) {
	return 0;
    }

    if(!parent.isValid()) {
	parentNode = rootDummy;
    } else {
	parentNode = static_cast<SceneGraph*>(parent.internalPointer());
    }
    return parentNode->childCount();
}

int  Scene::columnCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
	return static_cast<SceneGraph*>(parent.internalPointer())->columnCount();
    } else {
	return rootDummy->columnCount();
    }
}

QVariant Scene::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
	return QVariant();
    }
    if(role != Qt::DisplayRole) {
	return QVariant();
    }

    SceneGraph *node = static_cast<SceneGraph*>(index.internalPointer());

    return node->data(index.column());
}

Qt::ItemFlags Scene::flags(const QModelIndex &index) const {
    if(!index.isValid()) {
	return 0;
    } else {
	return QAbstractItemModel::flags(index);
    }
}

QVariant Scene::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant("Scene Model");
}


void Scene::setLightLocation(GLuint lightPositionLocation) {
    this->lightPositionLocation = lightPositionLocation;
}

QModelIndex Scene::addCube(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cube = new Cube();
    std::string name("Cube ");
    name += std::to_string(nextId());
    SceneGraph *s = new SceneGraph(cube, name);

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addCylinder(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cylinder = new Cylinder(tesselationLevel);
    std::string name("Cylinder ");
    name += std::to_string(nextId());
    SceneGraph *s = new SceneGraph(cylinder, name);

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addCone(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cone = new Cone(tesselationLevel);
    std::string name("Cone ");
    name += std::to_string(nextId());
    SceneGraph *s = new SceneGraph(cone, name);

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addSphere(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *sphere = new Sphere(tesselationLevel);
    std::string name("Sphere ");
    name += std::to_string(nextId());
    SceneGraph *s = new SceneGraph(sphere, name);

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addTorus(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *torus = new Torus(tesselationLevel);
    std::string name("Torus ");
    name += std::to_string(nextId());
    SceneGraph *s = new SceneGraph(torus, name);

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}


void Scene::draw(QMatrix4x4 cameraMatrix) {
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    modelViewMatrixStack.top() *= cameraMatrix;
    
    QVector4D lightDir = cameraMatrix * lightPosition;
    GLfloat lightDirArray[3] = {lightDir.x(), lightDir.y(), lightDir.z()};
    glUniform3fv(lightPositionLocation, 1, lightDirArray);
    
    this->rootNode->draw(modelViewMatrixStack, modelViewMatLocation, normalMatLocation);
    modelViewMatrixStack.pop();
}
