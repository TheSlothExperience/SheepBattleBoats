#define GL_GLEXT_PROTOTYPES

#include "projectilenode.h"
#include "glwidgetcontext.h"
#include "GL/gl.h"

ProjectileNode::ProjectileNode(QVector3D startPos,QVector3D shootingDir,Primitive *p, std::string name)
    :SceneGraph(p, name)
{
    this->startPos=startPos;
    this->name=name;
    this->shootDir=shootingDir;
    qDebug()<<"sDirX: "+QString::number(shootDir.x())+"sDirY: "+QString::number(shootDir.y())+"sDirZ: "+QString::number(shootDir.z());
}

ProjectileNode::~ProjectileNode(){
}


void ProjectileNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix) {
    draw(MVStack, cameraMatrix, projectionMatrix, Shaders::shaderProgram);
}

void ProjectileNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
//    qDebug()<<"Kugel";

    Shaders::bind(shader);
    MVStack.push(MVStack.top());

    MVStack.top().translate(this->translation);

    //Convert the quat to a matrix, may be a performance leak.
    QMatrix4x4 tempRot;
    tempRot.rotate(this->rotation.normalized());
    MVStack.top() *= tempRot;

    //If the node is a leaf, draw its contents
    if(leaf) {
        glUniformMatrix4fv(shader->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
        glUniformMatrix4fv(shader->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());
        glUniformMatrix4fv(shader->uniformLocation("normalMatrix"), 1, GL_FALSE, MVStack.top().inverted().transposed().constData());
        int r = (id & 0x000000FF) >>  0;
        int g = (id & 0x0000FF00) >>  8;
        int b = (id & 0x00FF0000) >> 16;
        glUniform4f(shader->uniformLocation("id"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

        glUniform4fv(shader->uniformLocation("color"), 1, color);

        this->primitive->draw();
    } else {
        //Else, recurse into its children
        std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->draw(MVStack, cameraMatrix, projectionMatrix, shader);});
    }

    MVStack.pop();
    Shaders::release(shader);
}

void ProjectileNode::exeObjBehaviour(){
//    qDebug()<<"exeObjBehaviour in projectileNode.";
    float t= livedTicks;
    if(livedTicks>=MAX_TICKSTOLIVE||translation.y()<-10.0){
//        qDebug()<<"Projectile shouldbe dead";
    }else{
        livedTicks+=0.1;
        translation=QVector3D(shootDir.x()*t+startPos.x(),
                              gravity*t*t+shootDir.y()*t+startPos.y(),
                              shootDir.z()*t+startPos.z());
//        setNewBBPosition(translation);

    }
}

BoundingBox* ProjectileNode::getBB(){
    BoundingBox* a=primitive->getBB();
//    qDebug()<<QString::number(translation.x());
    a->position=translation;

    return a;
}

void ProjectileNode::setNewPosition(QVector3D newPos){
    this->getBB()->setNewPosition(newPos);
}

void ProjectileNode::reactToCollision(){
    qDebug()<<"deleting row " << row();


    setDeadmark(true);
//    emit deleteNode(this);
}
