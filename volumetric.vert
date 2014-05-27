#version 330 core

out vec3 color;
out vec3 uv;

layout(location = 0) in vec4 position;
layout(location = 2) in vec3 texcoords;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;


void main(){
    // Transforming The Vertex Position To ModelView-Space
    gl_Position = perspectiveMatrix * modelViewMatrix * position;

    uv = texcoords + vec3(0.5);
    color = vec3(position) + vec3(0.5);
}
