#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <stack>
#include <string>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

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

    QQuaternion getRotation(){return this->rotation;}

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
	virtual void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc, GLuint idLoc);
	virtual void draw(std::stack<QMatrix4x4> &MVStack, GLuint mvLoc, GLuint normalLoc, GLuint idLoc, GLuint colorLoc);
//    virtual void testCollisions();
};

#endif //SCENE_H
