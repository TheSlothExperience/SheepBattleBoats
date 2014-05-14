#version 330
/*
 * Use compatibility mode so we can use
 * fixed-function convenience stuff too.
 */
//varying vec3 N;
//varying vec3 V;
//varying vec3 L;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec4 normalC;

uniform mat4 perspectiveMatrix;
uniform mat4 modelMatrix;

void main()
{
    //gl_Position = pvm * position;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 offset = vec4(-1.0, -1.5, -3.0, 0.0);
    gl_Position = (perspectiveMatrix * modelMatrix) * position;
    normalC = (normal);
    // Transforming The Normal To ModelView-Space
    //N = normalize(gl_NormalMatrix * gl_Normal);
    
    // Transforming The Vertex Position To ModelView-Space
    //V = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // Calculating The Vector From The Vertex Position To The Light Position
    //L = normalize(gl_LightSource[0].position.xyz - V);
    //gl_FrontColor = gl_Color;
} 
