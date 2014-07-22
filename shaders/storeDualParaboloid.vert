#version 330 core

out vec4 P;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

uniform float direction;
uniform vec4 id;

void main()
{
    vec4 outPosition = modelViewMatrix * position;
    outPosition /= outPosition.w;

    float L = length(outPosition.xyz);

    outPosition /= L;

    outPosition.z = outPosition.z + 1;
    outPosition.x = outPosition.x / outPosition.z;
    outPosition.y = outPosition.y / outPosition.z;

    outPosition.z = (L - 0.1) / (500.0 - 0.1);
    outPosition.w = 1;

    gl_Position = outPosition;
    P = outPosition;
}
