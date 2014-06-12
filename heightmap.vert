#version 440 core

out vec3 vPosition;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

void main()
{
    vPosition = position.xyz;
} 
