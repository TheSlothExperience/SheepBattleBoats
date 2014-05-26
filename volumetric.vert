#version 330 core

out vec3 color;

layout(location = 0) in vec4 position;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;


void main(){
    // Transforming The Vertex Position To ModelView-Space
    gl_Position = perspectiveMatrix * modelViewMatrix * position;
    
    color = vec3(position) + vec3(0.5);
}
