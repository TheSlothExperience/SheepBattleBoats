#define GL_GLEXT_PROTOTYPES

#include "glwidget.h"
#include <QtGui>
#include <GL/gl.h>
#include "GL/glu.h"
#include <math.h>
#include <iostream>

#include "perspectivecamera.h"
#include "orthocamera.h"

const int TEXTURE_WIDTH = 1024;
const int TEXTURE_HEIGHT = 768;

GLWidget::GLWidget(QWidget *parent, const QGLWidget *shareWidget)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shareWidget),
	  tesselationLevel(0),
	  zoom(0.0), dragging(false), satShadowsp(false)
{
	scene = NULL;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}

void GLWidget::initializeGL()
{
	//Set up OpenGL incantations
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_3D);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    //Ab hier, wenn Deferred Shading
    gbuffer = GBuffer();
    gbuffer.Init(TEXTURE_WIDTH,TEXTURE_HEIGHT);

	//Create the drawing quad
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
}


void GLWidget::paintGL()
{
	//Clear the buffers
    gbuffer.startFrame();
    //Render the Textures for DeferredShading
    DSGeometryPass();

    //Shadow map pass. Render them and blur
    shadowMapsPass();

    //Use of the Textures to Render to the Magic Quad
    DSLightPass();
}

/*
 * Render the SceneGraph without lighting and phong shading.
 * The result is painted into the the color attachments
 * of the FBO.
 */
void GLWidget::DSGeometryPass() {
    //Load the phong shading program
    shaders.shaderProgram->bind();

    glUniformMatrix4fv(shaders.shaderProgram->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, camera->getProjectionMatrix().constData());
    glViewport(0,0,1024,768);
    gbuffer.bindGeometryPass();

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if(scene != NULL) {
        //Discombobulate!
        scene->draw(camera->getCameraMatrix());
    } else {
        std::cout << "no scene yet" << std::endl;
    }

    glDepthMask(GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shaders.shaderProgram->release();
}

//Mix the textures and render the scene to the magical quad
void GLWidget::DSLightPass(){

    shaders.lightPassProgram->bind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,this->width(), this->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbuffer.bindLightPass(shaders.lightPassProgram);
    scene->passLights(camera->getCameraMatrix(), shaders.lightPassProgram);

    passShadowMaps(shaders.lightPassProgram, 8);

    //Draw our nifty, pretty quad
    glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3*2);

    glDisableVertexAttribArray(0);

    shaders.lightPassProgram->release();
}

void GLWidget::passShadowMaps(QOpenGLShaderProgram *shaderProgram, const int texOffset) {
	//Send all the lighting information and shadowmaps
	std::vector<GLfloat> lightViews = scene->lightViews();
	std::vector<GLfloat> lightPerspectives = scene->lightPerspectives();
	glUniformMatrix4fv(shaderProgram->uniformLocation("lightViews"), lightViews.size() / 16, GL_FALSE, lightViews.data());
	glUniformMatrix4fv(shaderProgram->uniformLocation("lightPerspectives"), lightPerspectives.size() / 16, GL_FALSE, lightPerspectives.data());

	//Now the shadowmaps
	std::vector<GLuint> shadowMapLocs = scene->shadowMapLocations();
	std::vector<GLuint> shadowSATLocs = scene->shadowSATs();
	GLint *shadowSamplers = new GLint[shadowMapLocs.size()];
	GLint *shadowSATs = new GLint[shadowSATLocs.size()];
	unsigned int shadowsOffset = texOffset;
	for(unsigned int i = 0; i < shadowMapLocs.size(); i++) {
		shadowSamplers[i] = i + shadowsOffset;
		shadowSATs[i] = i + shadowsOffset + shadowMapLocs.size();
		//Start at GL_TEXTURE0 + shadowsOffset
		glActiveTexture(GL_TEXTURE0 + shadowsOffset + i);
		glBindTexture(GL_TEXTURE_2D, shadowMapLocs[i]);
		glActiveTexture(GL_TEXTURE0 + shadowsOffset + shadowMapLocs.size() + i);
		glBindTexture(GL_TEXTURE_2D, shadowSATLocs[i]);
	}
	glUniform1iv(shaderProgram->uniformLocation("shadowMaps"), shadowMapLocs.size(), shadowSamplers);
	glUniform1iv(shaderProgram->uniformLocation("shadowSATs"), shadowSATLocs.size(), shadowSATs);
	delete[] shadowSamplers;

	//An additional bias matrix
	GLfloat bias[16] = {0.5, 0.0, 0.0, 0.5,
	                    0.0, 0.5, 0.0, 0.5,
	                    0.0, 0.0, 0.5, 0.5,
	                    0.0, 0.0, 0.0, 1.0};
	glUniformMatrix4fv(shaderProgram->uniformLocation("lightBias"), 1, GL_TRUE, bias);
	glUniformMatrix4fv(shaderProgram->uniformLocation("inverseCameraMatrix"), 1, GL_FALSE, camera->getCameraMatrix().inverted().constData());
}

void GLWidget::shadowMapsPass() {
	if(scene != NULL) {
        //Discombobulate!
	    scene->lightsPass(shaders.storeDepthProgram);
	    scene->blurShadowMaps(shaders.gaussianBlurHProgram, shaders.gaussianBlurVProgram);
	    if(satShadowsp) {
		    scene->computeSAT(NULL);
	    }
    } else {
        std::cout << "no scene yet" << std::endl;
    }
}

void GLWidget::resizeGL(int width, int height)
{
	this->camera->resize(width, height);
	glViewport(0, 0, width, height);
}

void GLWidget::setProjectionLocation(GLuint pL) {
	this->perspectiveMatLocation = pL;
}
void GLWidget::setCamera(Camera *camera) {
	this->camera = camera;
}

void GLWidget::setPerspectiveCamera(double x, double y, double z) {
	this->camera = new PerspectiveCamera(x, y, z, this->width(), this->height());
}

void GLWidget::setOrthoCamera(double x, double y, double z) {
	this->camera = new OrthoCamera(x, y, z, this->width(), this->height());
}

void GLWidget::setScene(Scene *scene) {
	this->scene = scene;
}

void GLWidget::setActive(bool active) {
	this->isActive = active;
	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		dragging = true;
	}
	if(event->button() == Qt::LeftButton) {
		//Picking
		//Make sure we sample from the fbo textures
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//Make sure we are done drawing
		glFlush();
		glFinish();

		//We want to sample the picking buffer
		glReadBuffer( GL_COLOR_ATTACHMENT1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//Make some room for the pixel data and transform into tex coords
		unsigned char data[4];
		int x = 1024 * (event->x() / (float) width());
		int y = 786 * ((height() - event->y()) / (float) height());
		;
		//Get dem pixorz!
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//Unpack the data into the id
		int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;
		if(pickedID < 100) { //The background color, have to make this more robust
			emit changedActiveId(pickedID);
			emit changedCurrent(scene->identify(pickedID));
		}
		//Leave as before
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	emit switchActive(this);
}

/*
 * Project coordinates to the surface of the sphere or to
 * the hyperbolic surface, if it doesn's lie on the sphere.
 */
double z(double x, double  y) {
	double length = sqrt(x*x + y*y);
	//Let the radius of the sphere be 1
	if (length <= 1.0/2.0) {
		return sqrt(1.0 - length);
	} else {
		return (1.0/2.0) / sqrt(length);
	}

}

double clampUnit(double x) {
	return std::min(1.0, std::max(-1.0, x));
}

QQuaternion M4toQuat(QMatrix4x4 mat) {
	double trace = mat(0,0) + mat(1,1) + mat(2,2) + 1;
	double s = 0.5 / sqrt(trace);
	double w = 0.25 / s;
	double x = (mat(2, 1) - mat(1, 2)) * s;
	double y = (mat(0, 2) - mat(2, 0)) * s;
	double z = (mat(1, 0) - mat(0, 1)) * s;
	return QQuaternion(w, x, y, z);
}

void GLWidget::translateCamera(double x, double y, double z) {
	QVector4D trans(x, y, z, 1.0);
	trans = trans * camera->getCameraMatrix();

	this->camera->translate(trans.x(), trans.y(), trans.z());
}

void GLWidget::rotateCamera(float angle) {
	QQuaternion rot = QQuaternion(cos(angle/2.0), sin(angle/2.0) * QVector3D(0.0, 1.0, 0.0));
	camera->rotate(rot);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::RightButton) && dragging) {
		double xtrans = (event->pos().x() - lastPoint.x()) / 1000.0;
		double ytrans = -(event->pos().y() - lastPoint.y()) / 1000.0; //Qt y-coord is inverted
		QVector4D trans(xtrans, ytrans, 0, 1);
		QVector4D worldTrans = trans * camera->getCameraMatrix();
		if(cameraActive) {
			this->camera->translate(-worldTrans.x(), -worldTrans.y(), -worldTrans.z());
			updateGL();
		} else {
			emit translate(worldTrans.x(), worldTrans.y(), worldTrans.z());
		}
	}

	if ((event->buttons() & Qt::LeftButton) && dragging) {
		//Here we implement the trackball. Sample two points on the sphere and
		//calculate their angle to use as the rotation.

		//normalize to intervals [-1,1]
		double lastx = clampUnit(lastPoint.x() / (this->size().width() / 2.0) - 1.0);
		double lasty = clampUnit(-(lastPoint.y() / (this->size().height() / 2.0) - 1.0));

		double newx = clampUnit(event->pos().x() / (this->size().width() / 2.0) - 1.0);
		double newy = clampUnit(-(event->pos().y() / (this->size().height() / 2.0) - 1.0));

		//Project the two points into the sphere (or the hyperbolic plane)
		QVector3D v1(lastx, lasty, z(lastx, lasty));
		v1.normalize();
		QVector3D v2(newx, newy, z(newx, newy));
		v2.normalize();

		//Determine the normal of the generated plane through the center of the sphere
		QVector3D normal = QVector3D::crossProduct(v1, v2);
		double theta = acos(QVector3D::dotProduct(v1, v2)) / 3.0;

		//angle/2.0, because the quats double cover SO(3)
		QQuaternion newRot = QQuaternion(cos(theta/2.0), sin(theta/2.0) * normal.normalized());
		QQuaternion cameraQuat = M4toQuat(camera->getCameraMatrix());
		QQuaternion worldQuat = cameraQuat.conjugate() * newRot * cameraQuat;
		if(cameraActive) {
			this->camera->rotate(newRot);
			updateGL();
		} else {
			emit rotate(&worldQuat);
		}
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton) {
		dragging = false;
	}
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	double zoom = event->delta()/300.0;
	this->camera->zoom(zoom);
	updateGL();
}

void GLWidget::setTesselation(int tesselationLevel) {
	this->tesselationLevel = tesselationLevel;
	updateGL();
}

void GLWidget::forceGLupdate() {
	updateGL();
}

void GLWidget::changeActiveId(int id) {
	this->activeID = id;
	updateGL();
}

void GLWidget::resetCamera() {
	if(isActive) {
		this->camera->reset();
		updateGL();
	}
}
