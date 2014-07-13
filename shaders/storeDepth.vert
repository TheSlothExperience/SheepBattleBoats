#version 440 core

out vec4 P;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

uniform vec4 id;

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;

    P = (perspectiveMatrix * modelViewMatrix) * position;
}
