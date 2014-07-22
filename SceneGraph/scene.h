#ifndef SCENE_H
#define SCENE_H

#include "scenegraph.h"
#include "lightnode.h"
#include "camera.h"
#include<levelobjectnode.h>
#include<projectilenode.h>

#include <stack>
#include <map>
#include <QMatrix4x4>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QOpenGLShader>

class Scene : public QAbstractItemModel
{
	Q_OBJECT

private:
	SceneGraph *rootNode;
	SceneGraph *rootDummy;


	GLuint modelViewMatLocation;
	GLuint normalMatLocation;
	GLuint lightPositionLocation;
	GLuint idLocation;
	GLuint colorLocation;

	std::map<int, SceneGraph*> identifier;

	static std::vector<LightNode*> lights;

	QVector4D lightPosition;
	std::stack<QMatrix4x4> modelViewMatrixStack;


	int currentId = 0;
	int nextId() {return currentId++;};

    LevelObjectNode* mainBoat;
    QList<SceneGraph*> levelObjAdresses;

public:
	Scene(QObject *parent = 0);
	Scene(GLuint mvLoc, GLuint normalLoc, GLuint idLoc, GLuint colorLoc, QObject *parent = 0);

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
	                  const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	//For editing
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	Qt::DropActions supportedDropActions() const;

	QModelIndex addCube(SceneGraph *node, int tesselationLevel);
	QModelIndex addCylinder(SceneGraph *node, int tesselationLevel);
	QModelIndex addCone(SceneGraph *node, int tesselationLevel);
	QModelIndex addSphere(SceneGraph *node, int tesselationLevel);
	QModelIndex addTorus(SceneGraph *node, int tesselationLevel);

	QModelIndex addGroup(SceneGraph *node);

	QModelIndex addLight();

    QModelIndex add3DModel(SceneGraph *node);
    QModelIndex addSea(SceneGraph *node);

	LevelObjectNode *addLevelObj();
    ProjectileNode *addProjectile(QVector3D shootingDir);

	void lightsPass();
	void blurShadowMaps(QOpenGLShaderProgram *hs, QOpenGLShaderProgram *vs);
	void computeSAT(QOpenGLShaderProgram *sat);

	std::vector<GLfloat> lightPerspectives();
	std::vector<GLfloat> lightViews();
	std::vector<GLuint> shadowMapLocations();
	std::vector<GLuint> shadowSATs();


	SceneGraph *root() {return rootNode;};

	QModelIndex identify(int i);
	void setLightLocation(GLuint lightPositionLocation);
	static void passLights(QMatrix4x4 cameraMatrix, QOpenGLShaderProgram *sp);

	void draw(Camera *camera);
    void DS_geometryPass(Camera *camera);

    void initLevel();
    void testCollisions();
    void doMovements();
    LevelObjectNode* getMainBoat(){return this->mainBoat;}
    QList<SceneGraph*> getLvlObjAdresses(){return this->levelObjAdresses;}
    void translateMotherSheep(QVector3D);
    void rotateMotherSheep();
    QVector3D convertToMotherSheepTranslation();
    void behaviourExecutions();
};

#endif //SCENE_H
