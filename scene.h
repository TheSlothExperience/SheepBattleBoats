#ifndef SCENE_H
#define SCENE_H

#include "scenegraph.h"

#include <stack>
#include <QMatrix4x4>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class Scene : public QAbstractItemModel
{
    Q_OBJECT
    
private:
    SceneGraph *rootNode;
    SceneGraph *rootDummy;

    
    GLuint modelViewMatLocation;
    GLuint normalMatLocation;
    GLuint lightPositionLocation;

    QVector4D lightPosition;
    std::stack<QMatrix4x4> modelViewMatrixStack;

public:
    Scene(QObject *parent = 0);
    Scene(GLuint mvLoc, GLuint normalLoc, QObject *parent = 0);
    
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    
    SceneGraph *root() {return rootNode;};
    void setLightLocation(GLuint lightPositionLocation);
    void draw(QMatrix4x4 cameraMatrix);
};

#endif //SCENE_H
