#version 440 core

out vec3 V;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;
    
    
    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);
} 
