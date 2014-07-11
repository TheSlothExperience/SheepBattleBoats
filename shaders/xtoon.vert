#version 440 core

out vec3 N;
out vec3 V;
out vec4 id4;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

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
