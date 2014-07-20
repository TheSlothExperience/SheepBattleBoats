#version 330

out vec3 N;
out vec3 V;
out vec4 W;
out vec2 texCoord0;
out vec4 id4;
out vec4 C;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoords;

uniform sampler3D noiseTexture;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 cameraInverseMatrix;

uniform int seaWidth;
uniform int seaHeight;

uniform float time;

uniform vec4 id;

void main()
{
	vec2 UV = position.xz / vec2(seaWidth, seaHeight);
	float height = texture(noiseTexture, vec3(UV, time)).r;
	vec4 V_ = vec4(position.x, height, position.z, 1.0);
    gl_Position = (perspectiveMatrix * modelViewMatrix) * V_;

    // Transforming The Normal To ModelView-Space
    N = normalize(vec3(normalMatrix * normal));

    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * V_);
    W = cameraInverseMatrix * modelViewMatrix * V_;

    //Weitergabe der Texturkoordinaten
    texCoord0 = texCoords;

    id4 = id;

    C = vec4(0.0, 0.0, 1.0, 1.0);
}
