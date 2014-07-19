#define GL_GLEXT_PROTOTYPES

#include "seanode.h"
#include "Util/Perlin.h"

#include <iostream>
#include <math.h>
#include <GL/gl.h>


SeaNode::SeaNode(Primitive *p, std::string name)
	: SceneGraph(p, name)
{
	seaWidth = 200;
	seaHeight = 200;
	//Create noise texture
	float frequency = 1.0 / pow(2.0, 5);
	noiseData = new GLfloat[seaWidth * seaHeight];
	for(int y = 0; y < seaHeight; y++) {
		for(int x = 0; x < seaWidth; x++) {
			noiseData[y * seaWidth + x] = scaled_raw_noise_2d(0.0, 1.0, x * frequency, y * frequency);
		}
	}

	glGenTextures(1, &noiseTexture);

	std::cout << "Noise tex: " << noiseTexture << std::endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, seaWidth, seaHeight, 0, GL_RED, GL_FLOAT, (void*) noiseData);
	glBindTexture(GL_TEXTURE_2D, 0);
}


SeaNode::~SeaNode() {
	glDeleteTextures(1, &noiseTexture);
}
