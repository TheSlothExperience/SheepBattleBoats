#define GL_GLEXT_PROTOTYPES

#include "scene.h"
#include "primitive.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"
#include "sphere.h"
#include "torus.h"
#include "light.h"

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include <iostream>

Scene::Scene(QObject *parent)
    : QAbstractItemModel(parent)
{
    this->modelViewMatrixStack.push(QMatrix4x4());
}
Scene::Scene(GLuint mvLoc, GLuint normalLoc, GLuint idLoc, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->modelViewMatLocation = mvLoc;
    this->normalMatLocation = normalLoc;
    this->idLocation = idLoc;
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
	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}

QVariant Scene::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant("Scene Model");
}


bool Scene::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
	return false;
    }
    
    SceneGraph *node = static_cast<SceneGraph*>(index.internalPointer());
    bool result = node->setData(index.column(), value);

    if (result) {
	emit dataChanged(index, index);
    }
    
    return result;
}

bool Scene::removeRows(int position, int rows, const QModelIndex &parent) {
    SceneGraph *parentItem = static_cast<SceneGraph*>(parent.internalPointer());
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

void Scene::setLightLocation(GLuint lightPositionLocation) {
    this->lightPositionLocation = lightPositionLocation;
}

QModelIndex Scene::identify(int id) {
    auto it = identifier.find(id);
    SceneGraph *s = it->second;
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addCube(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cube = new Cube();
    std::string name("Cube ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(cube, name);
    s->setId(id);
    identifier[id] = s;
    
    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addCylinder(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cylinder = new Cylinder(tesselationLevel);
    std::string name("Cylinder ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(cylinder, name);
    s->setId(id);
    identifier[id] = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addCone(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *cone = new Cone(tesselationLevel);
    std::string name("Cone ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(cone, name);
    s->setId(id);
    identifier[id] = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addSphere(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *sphere = new Sphere(tesselationLevel);
    std::string name("Sphere ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(sphere, name);
    s->setId(id);
    identifier[id] = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addTorus(SceneGraph *node, int tesselationLevel) {
    beginResetModel();
    Primitive *torus = new Torus(tesselationLevel);
    std::string name("Torus ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(torus, name);
    s->setId(id);
    identifier[id] = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addLight() {
    beginResetModel();
    Primitive *light = new Light();
    std::string name("Light ");
    int id = nextId();
    name += std::to_string(id);
    LightNode *s = new LightNode(light, name);
    s->setId(id);
    identifier[id] = s;

    rootNode->add(s);
    lights.push_back(s);
    endResetModel();

    std::cout << "We got lights: " << lights.size() << std::endl;
    return createIndex(s->row(), 0, s);
}


void Scene::draw(QMatrix4x4 cameraMatrix) {
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    modelViewMatrixStack.top() *= cameraMatrix;
    
    QVector4D lightDir = cameraMatrix * lightPosition;
    GLfloat lightDirArray[3] = {lightDir.x(), lightDir.y(), lightDir.z()};
    int lightNums = 1;
    glUniform3fv(shaderProgram->uniformLocation("lightPositions"), lightNums, lightDirArray);
    glUniform1i(shaderProgram->uniformLocation("numLights"), lightNums);
    
    this->rootNode->draw(modelViewMatrixStack, modelViewMatLocation, normalMatLocation, idLocation);
    modelViewMatrixStack.pop();
}
