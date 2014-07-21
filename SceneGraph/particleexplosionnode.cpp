#define GL_GLEXT_PROTOTYPES

#include "gbuffer.h"
#include "particleexplosionnode.h"
#include "glwidgetcontext.h"
#include "math.h";
#include <iostream>
ParticleExplosionNode::ParticleExplosionNode(QVector3D pos,Primitive *p,std::string name)
	:SceneGraph(p,name)
{
	this->emitterPos=pos;
	nParticles=30;
	for(int i=0;i<nParticles;i++){
		Particle temp=Particle();
		particles.push_back(temp);
	}
	//erzeugt ale Partikel
	emitParticles();
	buildVertexBuffer();

	std::string fileName = ":/img/sprites/explosion1.png";
	QImage tex;
	QString fileQT = QString(fileName.c_str());

	tex.load(fileQT);
	tex = QGLWidget::convertToGLFormat(tex);

	glGenTextures(1, &sprite);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}


ParticleExplosionNode::~ParticleExplosionNode(){}



void ParticleExplosionNode::draw(std::stack<QMatrix4x4> &MVStack, QMatrix4x4 cameraMatrix, QMatrix4x4 projectionMatrix, QOpenGLShaderProgram *shader) {
	//If the node is a leaf, draw its contents
	if(leaf) {
		GBuffer::activeGBuffer()->drawToFinal();
		Shaders::bind(Shaders::particleProgram);
		MVStack.push(MVStack.top());

		MVStack.top().translate(this->translation);

		//Convert the quat to a matrix, may be a performance leak.
		QMatrix4x4 tempRot;
		tempRot.rotate(this->rotation.normalized());
		MVStack.top() *= tempRot;

		glUniformMatrix4fv(Shaders::particleProgram->uniformLocation("modelViewMatrix"), 1, GL_FALSE, MVStack.top().constData());
		glUniformMatrix4fv(Shaders::particleProgram->uniformLocation("perspectiveMatrix"), 1, GL_FALSE, projectionMatrix.constData());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sprite);
		glUniform1i(Shaders::particleProgram->uniformLocation("sprite"), 0);

		drawParticles();

		MVStack.pop();
		Shaders::release(Shaders::particleProgram);
		GBuffer::activeGBuffer()->bindGeometryPass();
	} else {
		//Else, recurse into its children
		std::for_each(children.begin(), children.end(), [&MVStack, cameraMatrix, projectionMatrix, shader](SceneGraph *s){s->draw(MVStack, cameraMatrix, projectionMatrix, shader);});
	}
}



void ParticleExplosionNode::exeObjBehaviour(){

	emmiterLifetime+= 0.1;
	if(emmiterLifetime>=emmiterMaxLifetime){

		setDeadmark(true);
	}
	updateParticles( 0.1);
}

void ParticleExplosionNode::reactToCollision(){}
BoundingBox* ParticleExplosionNode::getBB(){}



void ParticleExplosionNode::updateParticles(float deltaTime){

	for ( unsigned int i = 0; i < particles.size(); ++i )
	{
		Particle& particle = particles[i];

		particle.time += deltaTime;
		if ( particle.time > particle.lifeTime)
		{
			emitParticle(particle);
		}

		float lifeRatio = particle.time/ particle.lifeTime;
		particle.velocity+= ( force * 0.01);
		particle.pos += ( particle.velocity * 0.1);
		particle.alpha=  lifeRatio ;
		particle.rotate= lerp(0.0f, 720.0f, lifeRatio );
		particle.size = lerp( 1.0f, 0.0f, lifeRatio );
	}

	buildVertexBuffer();
}

float ParticleExplosionNode::lerp(float a, float b, float ratio){
	return a*(1-ratio)+b*ratio;
}



//
void ParticleExplosionNode::randomizeParticles(){

}

//erzeugt n Partikel zu Beginn
void ParticleExplosionNode::emitParticles()
{
	for ( unsigned int i = 0; i < nParticles; ++i )
	{
		emitParticle(particles[i]);
	}
}


void ParticleExplosionNode::emitParticle( Particle& particle )
{
	float x = (((rand()%1000)/1000.0)*maxWidth)-(maxWidth/2.0);

	float y = (((rand()%1000)/1000.0)*maxHeight)-(maxHeight/2.0);
	float z = (((rand()%1000)/1000.0)*maxDepth)-(maxDepth/2.0);

	float lifetime = (((rand()%1000)/1000.0)*maxLifeTime);

	float speed = (((rand()%1000)/1000.0)*maxSpeed);
	QVector3D vector( x, y, z );

	particle.pos= vector + emitterPos;
	particle.velocity= vector.normalized() * speed;

	particle.lifeTime= lifetime;
	particle.time= 0;
}

void ParticleExplosionNode::buildVertexBuffer(){

	QVector3D x = QVector3D(0.2, 0, 0);
	QVector3D y = QVector3D( 0, 0.2, 0 );
	QVector3D z = QVector3D( 0, 0 ,1.0 );

	QQuaternion cameraRotation;

	GLfloat *vertices = new GLfloat[particles.size()*4*3];
	GLfloat *texCoords = new GLfloat[particles.size()*4*2];
	GLfloat *alpha= new GLfloat[particles.size()* 4*1];

	for ( unsigned int i = 0; i < particles.size(); ++i )
	{
		Particle& particle = particles[i];

		vertices[i*12 + 0] =(particle.pos+  (-x - y)).x();
		vertices[i*12 + 1] =(particle.pos+ (-x-y)).y();
		vertices[i*12 + 2] =(particle.pos+ (-x - y )).z();// Bottom-left

		vertices[i*12 + 3] =(particle.pos+  ( x - y )) .x();
		vertices[i*12 + 4] =(particle.pos+  ( x - y )).y();
		vertices[i*12 + 5] =(particle.pos+  ( x - y )).z();   // Bottom-right


		vertices[i*12 + 6] =(particle.pos+ ( x + y )).x();
		vertices[i*12 + 7] =(particle.pos+ ( x + y )).y();
		vertices[i*12 + 8] =(particle.pos+  ( x + y )).z();   // Top-right

		vertices[i*12 + 9] =(particle.pos+  ( -x + y )).x();
		vertices[i*12 + 10] =(particle.pos+ ( -x + y )).y();
		vertices[i*12 + 11] =(particle.pos+  ( -x + y )).z();   // Top-left

		texCoords[i *8 + 0]=0.0;
		texCoords[i *8 + 1]=1.0;
		texCoords[i *8 + 2]=1.0;
		texCoords[i *8 + 3]=1.0;
		texCoords[i *8 + 4]=1.0;
		texCoords[i *8 + 5]=0.0;
		texCoords[i *8 + 6]=0.0;
		texCoords[i *8 + 7]=0.0;


		alpha[i*4 +0]=particle.alpha;
		alpha[i*4 +1]=particle.alpha;
		alpha[i*4 +2]=particle.alpha;
		alpha[i*4 +3]=particle.alpha;

	}


	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*particles.size()* 4*3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &texCoordsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*particles.size()* 4*2, texCoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &alphaBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, alphaBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*particles.size()* 4*1, alpha, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ParticleExplosionNode::drawParticles() {
	//Bind the arrays to the vao
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferObject);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, alphaBufferObject);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_QUADS, 0, 4*nParticles);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
