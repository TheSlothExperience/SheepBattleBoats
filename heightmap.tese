#version 440 core

layout(quads) in;

in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;

out vec3 teTc;

uniform sampler2D heightMapTexture;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

in float mipLevel[];

const float maxMipLevel = log2(64);
const float stepSize = 1.0 / 51.0;
uniform float heightScale;
uniform float terrainSize;

vec2 toCoords(vec2 pos)
{
	vec4 t = modelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0);
	return ((pos - t.xz)/terrainSize + 25.0) / 51.0;
}

float heightLod(vec2 pos, float lod)
{
	return textureLod(heightMapTexture, toCoords(pos), lod).r;
}

vec3 samplePosition(float x, float z, float lod)
{
	return vec3(x, heightLod(vec2(x, z), lod), z) * vec3(1.0, heightScale, 1.0);
}

vec3 normal(vec3 pos, float lod)
{
	float x = pos.x;
	float z = pos.z;
	vec3 H = pos;
	vec3 H_x = samplePosition(x + stepSize, z, lod);
	vec3 H_z = samplePosition(x, z + stepSize, lod);

	vec3 N = cross(H_z - H, H_x - H);
	return normalize(N);
}

vec4 project(vec3 pos) {
	vec4 p =  modelViewMatrix * vec4(pos, 0.0);
	vec4 t = modelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0);
	vec4 rt = vec4(vec3(transpose(modelViewMatrix) * t), 1.0);
	p += vec4(vec3(0.0, rt.y, 0.0), 1.0);
	return p;
}

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	float a = mix(mipLevel[0], mipLevel[1], u);
	float b = mix(mipLevel[2], mipLevel[3], u);
	float lod = maxMipLevel - mix(a, b, v);

	//mix() does linear interpolation
	//here we are just doing some lerping
	vec3 s = mix(tcPosition[0], tcPosition[1], u);
	vec3 t = mix(tcPosition[2], tcPosition[3], u);

	tePosition = mix(s, t, v);
	tePosition = samplePosition(tePosition.x, tePosition.z, lod);
	tePatchDistance = vec4(u, v, 1 - u, 1 - v);
	teTc = vec3(tePosition);

	gl_Position = perspectiveMatrix * project(tePosition);
}
