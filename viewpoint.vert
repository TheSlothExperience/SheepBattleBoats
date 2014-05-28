#version 330

out vec3 V;
out vec3 color;

layout(location = 0) in vec4 position;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

uniform vec4 id;

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;
    
    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);

    //Norm to unit cube (radius = 0.5)
    color = vec3(sign(position.x), sign(position.y), sign(position.z)) * 0.5 + vec3(0.5);
} 
