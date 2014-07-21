#version 440 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in float alpha;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

out vec2 vTexCoords;
out float vAlpha;

void main(void)
{
	gl_Position = (perspectiveMatrix * modelViewMatrix) * position;
	vTexCoords = texCoords;
	vAlpha=alpha;
}
