#version 440 core
#pragma optionNV unroll all

out vec3 N;
out vec3 V;
out vec4 id4;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

uniform mat4 lightViews[maxLights];
uniform mat4 lightPerspectives[maxLights];
uniform sampler2D shadowMaps[maxLights];
uniform mat4 lightBias;



uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 inverseCameraMatrix;

uniform vec4 id;

//uniform vec3 lightPosition;

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;

    // Transforming The Normal To ModelView-Space
    N = normalize(vec3(normalMatrix * normal));

    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);


    id4 = id;
}
