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

	cubeMapLocation = loadCubemap();
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};
	glGenBuffers (1, &skyBox);
	glBindBuffer (GL_ARRAY_BUFFER, skyBox);
	glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GLWidget::paintGL()
{
	//Clear the buffers
    gbuffer.startFrame();

    //Draw the sky
    drawSkyBox();

    //Render the Textures for DeferredShading
    DSGeometryPass();

    //Shadow map pass. Render them and blur
    shadowMapsPass();

    //Use of the Textures to Render to the Magic Quad
    DSLightPass();

    getSceneIntensity();
    blurIntensity();

    paintSceneToCanvas();

}

/*
 * Render the SceneGraph without lighting and phong shading.
 * The result is painted into the the color attachments
 * of the FBO.
 */
void GLWidget::DSGeometryPass() {
    //Load the phong shading program
	Shaders::bind(Shaders::shaderProgram);

    glViewport(0,0,1024,768);
    gbuffer.bindGeometryPass();

    if(scene != NULL) {
        //Discombobulate!
	    scene->draw(camera);
    } else {
        std::cout << "no scene yet" << std::endl;
    }

    glDepthMask(GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Shaders::release(shaders.shaderProgram);
}

/* Mix the textures and render the scene to the magical quad
 * located in the finalTexture of the GBuffer
 */
void GLWidget::DSLightPass(){

	Shaders::bind(shaders.lightPassProgram);

    gbuffer.bindLightPass(shaders.lightPassProgram);
    Scene::passLights(camera->getCameraMatrix(), shaders.lightPassProgram);

    passShadowMaps(shaders.lightPassProgram, 8);

    //Draw our nifty, pretty quad
    glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3*2);

    glDisableVertexAttribArray(0);

    Shaders::release(shaders.lightPassProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Get the rendered texture from the GBuffer and paint it to
 * the screen
 */
void GLWidget::paintSceneToCanvas() {
	Shaders::bind(shaders.canvasProgram);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(8.0f, 8.0f, 8.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0, this->width(), this->height());
    gbuffer.bindFinalPass(shaders.canvasProgram);

    //blurred Intensity to da Vader
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(shaders.canvasProgram->uniformLocation("blurredIntensity"),1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.getTempTexture(2));

    //Draw our nifty, pretty quad
    glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3*2);

    glDisableVertexAttribArray(0);

    Shaders::release(shaders.canvasProgram);
}

void GLWidget::getSceneIntensity(){

    Shaders::bind(shaders.intensityProgram);

    gbuffer.tempTexture(0);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shaders.intensityProgram->uniformLocation("scene"),0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.getFinalLocation());

    //Draw our nifty, pretty quad
    glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3*2);

    glDisableVertexAttribArray(0);
    Shaders::release(shaders.intensityProgram);
}

void GLWidget::blurIntensity(){

    getSceneIntensity();

    //First pass, horizontal
    {
        Shaders::bind(shaders.gaussianBlurHProgram);

        gbuffer.tempTexture(1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer.getTempTexture(0));
        glUniform1i(shaders.gaussianBlurHProgram->uniformLocation("moments"), 0);

        //Draw our nifty, pretty quad
        glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3*2);

        glDisableVertexAttribArray(0);

        Shaders::release(shaders.gaussianBlurHProgram);
    }

    //Second pass, vertical into the shadow map
    {
        Shaders::bind(shaders.gaussianBlurVProgram);
        gbuffer.tempTexture(2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer.getTempTexture(1));
        glUniform1i(shaders.gaussianBlurVProgram->uniformLocation("moments"), 0);

        //Draw our nifty, pretty quad
        glBindBuffer(GL_ARRAY_BUFFER, canvasQuad);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3*2);

        glDisableVertexAttribArray(0);

        Shaders::release(shaders.gaussianBlurVProgram);
    }

}

void GLWidget::drawSkyBox() {
	GBuffer::activeGBuffer()->drawToFinal();
	glDepthMask(GL_FALSE);

	Shaders::bind(Shaders::skyBoxProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapLocation);
	glUniform1i(Shaders::skyBoxProgram->uniformLocation("cubeTex"), 0);

	glUniformMatrix4fv(Shaders::skyBoxProgram->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, camera->getProjectionMatrix().data());
	glUniformMatrix4fv(Shaders::skyBoxProgram->uniformLocation("modelViewMatrix"), 1, GL_FALSE, camera->getCameraMatrix().data());

	glBindBuffer(GL_ARRAY_BUFFER, skyBox);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);

	glDepthMask(GL_TRUE);
	Shaders::release(Shaders::skyBoxProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GLWidget::loadCubemap(){

	unsigned int tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);

	std::string fileName[6];
	fileName[0] = ":/img/skybox/right.png";
	fileName[1] = ":/img/skybox/left.png";
	fileName[2] = ":/img/skybox/bottom.png";
	fileName[3] = ":/img/skybox/top.png";
	fileName[4] = ":/img/skybox/front.png";
	fileName[5] = ":/img/skybox/back.png";

	for(int i=0;i<6;i++)
	{

		if(!fileName[i].empty()){
			QImage tex;
			QString fileQT = QString(fileName[i].c_str());
			tex.load(fileQT);
			tex = QGLWidget::convertToGLFormat(tex);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,tex.width(),tex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex.bits());
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		} else {
			std::cout << "Error loading texture '" << fileName[i] << std::endl;
			return 0;
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return tex;
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

void GLWidget::translateBoardCamera(QVector3D trans){
// trans = trans * camera->getCameraMatrix();
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

//	if ((event->buttons() & Qt::LeftButton) && dragging) {
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
//	}
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
