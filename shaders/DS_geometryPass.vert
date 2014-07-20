#version 330

out vec3 N;
out vec3 V;
out vec2 texCoord0;
out vec4 id4;
out vec4 C;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoords;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

uniform vec4 id;

uniform vec4 color;

//uniform vec3 lightPosition;

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;

    // Transforming The Normal To ModelView-Space
    N = normalize(vec3(normalMatrix * normal));

    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);


    //Weitergabe der Texturkoordinaten
    texCoord0 = vec2(texCoords.x, 1.0 - texCoords.y);

    id4 = id;

    C = color;
}
