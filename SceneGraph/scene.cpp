#define GL_GLEXT_PROTOTYPES

#include "scene.h"
#include "primitive.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"
#include "sphere.h"
#include "torus.h"
#include "light.h"
#include "object3d.h"
#include "sea.h"
#include "seanode.h"
#include "Reduction.h"
#include "levelobject.h"
#include "projectile.h"
#include "collisiondetection.h"
#include "particlegenerator.h"

#include "glwidget.h"
#include "glwidgetcontext.h"
#include <QtGui>
#include <GL/gl.h>
#include <iostream>

std::vector<LightNode*> Scene::lights = vector<LightNode*>();

Scene::Scene(QObject *parent)
	: QAbstractItemModel(parent)
{
	this->modelViewMatrixStack.push(QMatrix4x4());
}

Scene::Scene(GLuint mvLoc, GLuint normalLoc, GLuint idLoc, GLuint colorLoc, QObject *parent)
	: QAbstractItemModel(parent)
{
	this->modelViewMatLocation = mvLoc;
	this->normalMatLocation = normalLoc;
	this->idLocation = idLoc;
	this->colorLocation = colorLoc;
	this->modelViewMatrixStack.push(QMatrix4x4());

	this->rootDummy = new SceneGraph();
	this->rootNode = new SceneGraph();
	this->rootDummy->add(rootNode);

	lightPosition = QVector4D(0.5, 0.0, 2.0, 1.0);

	addLight();
	lights.at(0)->translate(1.0, 3.0, 1.50);
	//addTorus(rootNode, 8);
	//addSea(rootNode);
    levelObjAdresses= QList<SceneGraph*>();
//	addTorus(rootNode, 8);
}

void Scene::initLevel(){
    SheepNode *temp = addMainSheep();
    mainBoat=temp;

    addSea(rootNode);
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
		if(static_cast<SceneGraph*>(index.internalPointer())->isLeaf()) {
			return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable |
				Qt::ItemIsDragEnabled;

		} else {
			return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable |
				Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
		}
	}
}

Qt::DropActions Scene::supportedDropActions() const
{
	return Qt::MoveAction;
}

QVariant Scene::headerData(int, Qt::Orientation, int) const {
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

bool Scene::insertRows(int position, int rows, const QModelIndex &parent) {
	SceneGraph *parentItem = static_cast<SceneGraph*>(parent.internalPointer());
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootNode->columnCount());
	endInsertRows();

	return success;
}

bool Scene::removeRows(int position, int rows, const QModelIndex &parent) {
	SceneGraph *parentItem = static_cast<SceneGraph*>(parent.internalPointer());
	bool success = true;

	for(int i = 0; i < rows; i++) {
		SceneGraph *toRemove = parentItem->child(position + i);
		unsigned int found = std::find(lights.begin(), lights.end(), toRemove) - lights.begin();
		if(found < lights.size()) {
			lights.erase(lights.begin() + found);
		}
	}
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

QModelIndex Scene::addCube(SceneGraph *node, int) {
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

QModelIndex Scene::addGroup(SceneGraph *node) {
	beginResetModel();
	std::string name("Group ");
	int id = nextId();
	name += std::to_string(id);
	SceneGraph *s = new SceneGraph(false);
	s->setId(id);
	s->setName(name);
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

	return createIndex(s->row(), 0, s);
}

QModelIndex Scene::add3DModel(SceneGraph *node){
    beginResetModel();
    Object3D*object3d = new Object3D();
    object3d->loadMesh(":/models/sheep.obj",false);
    object3d->draw();
    std::string name("Object ");
    int id = nextId();
    name += std::to_string(id);
    SceneGraph *s = new SceneGraph(object3d,name);
    s->setId(id);
    identifier[id] = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

QModelIndex Scene::addSea(SceneGraph *node){
    beginResetModel();
    Primitive *seaP = new Sea();
    std::string name("Sea of Moist Wetness ");
    int id = nextId();
    name += std::to_string(id);
    SeaNode *s = new SeaNode(seaP, name);
    s->setId(id);
    identifier[id] = s;

    sea = s;

    node->add(s);
    endResetModel();
    return createIndex(s->row(), 0, s);
}

void Scene::draw(Camera *camera) {
	modelViewMatrixStack.push(modelViewMatrixStack.top());
	modelViewMatrixStack.top() *= camera->getCameraMatrix();

	this->rootNode->draw(modelViewMatrixStack, camera->getCameraMatrix(), camera->getProjectionMatrix());
	modelViewMatrixStack.pop();
}

SheepNode* Scene::addMainSheep(){
    beginResetModel();
    QQuaternion around = QQuaternion(cos(1.57), sin(1.57) * QVector3D(0.0, 1.0, 0.0));
    Object3D*object3d = new Object3D();
    object3d->loadMesh(":/models/sheep.obj",false);
    object3d->draw();
    std::string name("Master Sheep ");
    int id = nextId();

    SheepNode *s = new SheepNode(object3d,name, around.normalized());
    s->setId(id);
    identifier[id] = s;

    mainBoat = s;
    rootNode->add(s);
    levelObjAdresses.append(s);
    return s;
    endResetModel();
}

ProjectileNode* Scene::addProjectile(QVector3D shootingDir){
    QVector3D temp=mainBoat->getBB()->position;

    Projectile *lvlObj = new Projectile(temp);
    std::string name("Projectile ");
    int id = nextId();
    name+=std::to_string(id);
    ProjectileNode *s = new ProjectileNode(temp,shootingDir,lvlObj,name);
    QVector3D temp2 =mainBoat->getTranslation();
    s->translate(temp2.x(),temp2.y(),temp2.z());
    s->rotate(mainBoat->getRotation());
    s->setId(id);
    identifier[id] = s;
    rootNode->add(s);
    levelObjAdresses.append(s);
    return s;
}

ParticleExplosionNode* Scene::addParticleExplosionNode(QVector3D pos){
    ParticleGenerator *lvlObj = new ParticleGenerator();
    std::string name("Particle ");
    int id = nextId();
    name+=std::to_string(id);
    ParticleExplosionNode *s = new ParticleExplosionNode(pos,lvlObj,name);
    s->setId(id);
    identifier[id] = s;
    rootNode->add(s);
    levelObjAdresses.append(s);
    return s;
}

void Scene::DS_geometryPass(Camera *camera){
    modelViewMatrixStack.push(modelViewMatrixStack.top());
    modelViewMatrixStack.top() *= camera->getCameraMatrix();

    this->rootNode->draw(modelViewMatrixStack, camera->getCameraMatrix(), camera->getProjectionMatrix());
    modelViewMatrixStack.pop();
}


void Scene::passLights(QMatrix4x4 cameraMatrix, QOpenGLShaderProgram *sp) {
	//Copy the lights positions into GL friendly arrays
	GLfloat *lightsArray = new GLfloat[3 * lights.size()];
	GLfloat *colorsArray = new GLfloat[4 * lights.size()];
	for(unsigned int i = 0; i < lights.size(); i++) {
		QVector3D lightDir = cameraMatrix * lights.at(i)->getPosition();
		lightsArray[3 * i] = lightDir.x();
		lightsArray[3 * i + 1] = lightDir.y();
		lightsArray[3 * i + 2] = lightDir.z();

		GLfloat *color = lights.at(i)->getColor();
		colorsArray[4 * i] = color[0];
		colorsArray[4 * i + 1] = color[1];
		colorsArray[4 * i + 2] = color[2];
		colorsArray[4 * i + 3] = color[3];
	}
	glUniform3fv(sp->uniformLocation("lightPositions"), lights.size(), lightsArray);
	glUniform4fv(sp->uniformLocation("lightColors"), lights.size(), colorsArray);
	glUniform1i(sp->uniformLocation("numLights"), lights.size());

	delete[] lightsArray;
	delete[] colorsArray;
}

void Scene::lightsPass(QOpenGLShaderProgram *shader) {
	Shaders::bind(shader);
	for(auto l : lights) {
		glBindFramebuffer(GL_FRAMEBUFFER, l->shadowFBO());

		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1, DrawBuffers);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0, l->shadowWidth(), l->shadowHeight());

		modelViewMatrixStack.push(modelViewMatrixStack.top());
		modelViewMatrixStack.top() *= l->lightView();

		this->rootNode->drawGeometry(modelViewMatrixStack
		                   , l->lightView()
		                   , l->perspectiveMatrix()
		                   , shader);

		modelViewMatrixStack.pop();

		//Recreate the mipmaps
		glBindTexture(GL_TEXTURE_2D, l->shadowMoments());
		glGenerateMipmap(GL_TEXTURE_2D);

		//Release and relax, brah
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Shaders::release(shader);
}

void Scene::computeSAT(QOpenGLShaderProgram *) {
	for(auto l : lights) {
		glBindFramebuffer(GL_FRAMEBUFFER, l->shadowFBO());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Reduction::instance()->computeSATGLTexture(l->getShadowMap(), l->shadowMoments(), l->shadowMomentsTemp(), l->shadowWidth(), l->shadowHeight());

		//Release and relax, brah
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void Scene::blurShadowMaps(QOpenGLShaderProgram *hs, QOpenGLShaderProgram *vs) {
	GLuint canvasQuad;
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &canvasQuad);
	glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(auto l : lights) {
		glBindFramebuffer(GL_FRAMEBUFFER, l->shadowFBO());

		//First pass, horizontal
		{
			Shaders::bind(hs);
			GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT2};
			glDrawBuffers(1, DrawBuffers);

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0,0, l->shadowWidth(), l->shadowHeight());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, l->shadowMoments());
			glUniform1i(hs->uniformLocation("moments"), 0);

			//Draw our nifty, pretty quad
			glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, 3*2);

			glDisableVertexAttribArray(0);

			//Recreate the mipmaps
			glBindTexture(GL_TEXTURE_2D, l->shadowMomentsTemp());
			glGenerateMipmap(GL_TEXTURE_2D);
			Shaders::release(hs);
		}

		//Second pass, vertical into the shadow map
		{
			Shaders::bind(vs);
			GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1, DrawBuffers);

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0,0, l->shadowWidth(), l->shadowHeight());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, l->shadowMomentsTemp());
			glUniform1i(vs->uniformLocation("moments"), 0);

			//Draw our nifty, pretty quad
			glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, 3*2);

			glDisableVertexAttribArray(0);

			//Recreate the mipmaps
			glBindTexture(GL_TEXTURE_2D, l->getShadowMap());
			glGenerateMipmap(GL_TEXTURE_2D);
			Shaders::release(vs);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glDeleteBuffers(1, &canvasQuad);
}

/* Takes a nested functor `F (F A)` from `start` to
 * `end` and gives the flatten functor `F A` in `dest`.
 */
template <class COCiter, class Oiter>
void flatten(COCiter start, COCiter end, Oiter dest) {
    using namespace std;
    while (start != end) {
        dest = copy(start->begin(), start->end(), dest);
        ++start;
    }
}

/*
 * Takes a `v :: vector<T>` and a function `f :: T -> U` and
 * returns a `vector<U>` of the result of `f` on every element.
 */
template <typename U, typename T, class UnaryFunction>
std::vector<U> fmap(std::vector<T> v, UnaryFunction f) {
	std::vector<U> w;
	std::transform(v.begin(), v.end(), std::back_inserter(w), f);
	return w;
}

/* Return a flattened vector with the entries on the matrices.
 * Of size n * 16.
 */
std::vector<GLfloat> Scene::lightPerspectives() {
    using namespace std;
	vector<vector<GLfloat> > views = fmap<vector<GLfloat> >(lights, [=](LightNode *l){
			GLfloat* va = l->perspectiveMatrix().data();
			vector<GLfloat> v;
			v.assign(va, va + 16);
			return v;
		});
	vector<GLfloat> vs;
	flatten(views.begin(), views.end(), back_inserter(vs));
	return vs;
}

/* Return a flattened vector with the entries on the matrices.
 * Of size n * 16.
 */
std::vector<GLfloat> Scene::lightViews() {
    using namespace std;
	vector<vector<GLfloat> > views = fmap<vector<GLfloat> >(lights, [=](LightNode *l){
			GLfloat* va = l->lightView().data();
			vector<GLfloat> v;
			v.assign(va, va + 16);
			return v;
		});
	vector<GLfloat> vs;
	flatten(views.begin(), views.end(), back_inserter(vs));
	return vs;
}

std::vector<GLuint> Scene::shadowMapLocations() {
	return fmap<GLuint>(lights, [=](LightNode *l){return l->getShadowMap();});
}

std::vector<GLuint> Scene::shadowSATs() {
	return fmap<GLuint>(lights, [=](LightNode *l){return l->shadowMomentsTemp();});
}

void Scene::testCollisions(){
    for(int i=0;i<levelObjAdresses.length()-1;i++){

        for(int j=i+1;j<levelObjAdresses.length();j++){

            BoundingBox* bb1= levelObjAdresses.at(i)->getBB();
            BoundingBox* bb2= levelObjAdresses.at(j)->getBB();

            bool boatInvolved=mainBoat==levelObjAdresses.at(i)||mainBoat==levelObjAdresses.at(j);
            if(CollisionDetecion::isCollision(bb1,bb2)==1
               && !boatInvolved){
	            levelObjAdresses.at(i)->reactToCollision();
	            levelObjAdresses.at(j)->reactToCollision();
	            addParticleExplosionNode((bb1->position-bb2->position)+bb1->position);
            } else {
            }
        }
    }
    int adressCount= levelObjAdresses.length();
    for(int i=adressCount-1;i>=0;i--){
        if(levelObjAdresses.at(i)->isMarkedDead()){
            SceneGraph* adress=levelObjAdresses.at(i);
            adress->parent()->removeChildren(adress->row(),1);
            levelObjAdresses.removeAt(i);
            points+=100;
            qDebug()<<"Aktuelle Punktzahl: "<<points;
        }
    }
}

QVector3D Scene::convertToMotherSheepTranslation(){
    return  mainBoat->getRotation().rotatedVector(mainBoat->getVelocity());
}

void Scene::translateMotherSheep(QVector3D dir){
    mainBoat->translate(dir.x(),dir.y(),dir.z());
}

void Scene::translateSea(QVector3D dir){
    sea->translate(dir.x(),dir.y(),dir.z());
}

void Scene::rotateMotherSheep(){
    QQuaternion rot = QQuaternion(cos(mainBoat->getSteeringValue()/2.0), sin(mainBoat->getSteeringValue()/2.0) * QVector3D(0.0, 1.0, 0.0));
    mainBoat->rotate(rot);
}

void Scene::behaviourExecutions(){
    for(int i=0;i<levelObjAdresses.length();i++){
        levelObjAdresses.at(i)->exeObjBehaviour();
    }
}
