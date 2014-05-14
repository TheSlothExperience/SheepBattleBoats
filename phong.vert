#version 330

out vec3 N;
out vec3 V;
out vec3 L;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPosition;

void main()
{
    //gl_Position = pvm * position;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;
    
    // Transforming The Normal To ModelView-Space
    N = vec3(normalize(normalMatrix * normal));
    
    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);
    
    // Calculating The Vector From The Vertex Position To The Light Position
    L = normalize(lightPosition - V);
    //gl_FrontColor = gl_Color;
} 
