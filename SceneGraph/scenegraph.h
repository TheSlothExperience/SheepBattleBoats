#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QOpenGLShader>

#include "primitive.h"

class SceneGraph
{
protected:
	std::string name;
	bool leaf = false;
	int id;

	QQuaternion rotation;
	QVector3D translation;

	Primitive *primitive;
	SceneGraph *parentNode;

	std::vector<SceneGraph* > children;
	GLfloat color[4];
    bool markedDead=false;

public:
    SceneGraph(bool isleaf = false, SceneGraph *parent = 0);
    SceneGraph(Primitive *p, SceneGraph *parent = 0);
    SceneGraph(Primitive *p, std::string name);
	virtual ~SceneGraph();

	SceneGraph* parent();

	SceneGraph* child(int row);
	int childCount();
	int columnCount();
	QVariant data(int column);
	bool setData(int column, const QVariant &value);
	bool removeChildren(int position, int count);
	bool insertChildren(int position, int count, int columns);
	int row();

    QQuaternion getRotation();
    QVector3D getTranslation();

	void setParent(SceneGraph *s);

	void translate(double x, double y, double z);
	void rotate(QQuaternion q);

	virtual void changeColor(float, float, float, float) {}
	GLfloat* getColor() {return this->color;}

	void setId(int id) {this->id = id;}
	int getId() {return this->id;}
	std::string getName() {return name;};
	void setName(std::string name);
	bool isLeaf() {return leaf;};
	SceneGraph* add(Primitive *p);
	SceneGraph* add(SceneGraph *s);;

	virtual void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix);
	virtual void draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);
	virtual void drawGeometry(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader);

    virtual void exeObjBehaviour();
    virtual BoundingBox *getBB();
    virtual void reactToCollision(){}
    void setDeadmark(bool dead){this->markedDead=dead;}
    bool isMarkedDead(){return this->markedDead;}

    Primitive* getPrimitive(){return this->primitive;}


//signals:
//    void deleteNode(SceneGraph* nodeAdress);
};

#endif //SCENE_H
