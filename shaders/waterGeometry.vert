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


vec3 samplePosition(float x, float z)
{
	return vec3(x, texture(noiseTexture, vec3(x / seaWidth, z / seaHeight, time)).r, z);
}

void main()
{
	float x = position.x;
	float y = position.z;

	vec4 V_ = vec4(samplePosition(x, y), 1.0);
	vec3 V_x = samplePosition(x + 0.5, y);
	vec3 V_y = samplePosition(x, y + 0.5);

	vec3 n = normalize(cross(V_y - V_.xyz, V_x - V_.xyz));

    gl_Position = (perspectiveMatrix * modelViewMatrix) * V_;

    // Transforming The Normal To ModelView-Space
    N = normalize(vec3(normalMatrix * vec4(n, 1.0)));

    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * V_);
    W = cameraInverseMatrix * modelViewMatrix * V_;

    //Weitergabe der Texturkoordinaten
    texCoord0 = texCoords;

    id4 = id;

    C = vec4(0.0, 0.0, 1.0, 1.0);
}
