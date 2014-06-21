#version 440 core

layout(quads) in;

in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;

out vec3 color;
out vec3 tc;

uniform sampler2D heightMapTexture;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

const float stepSize = 1.0 / 51.0;
const float heightScale = 100.0;

vec2 toCoords(vec2 pos)
{
	return (pos + 25.0) / 51.0;
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

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	//mix() does linear interpolation
	//here we are just doing some lerping
	vec3 s = mix(tcPosition[0], tcPosition[1], u);
	vec3 t = mix(tcPosition[2], tcPosition[3], u);

	tePosition = mix(s, t, v);
	tePosition = samplePosition(tePosition.x, tePosition.z, 0);
	tePatchDistance = vec4(u, v, 1 - u, 1 - v);
	tc = vec3(tePosition.x, tePosition.z, tePosition.y);

	color = vec3(0.2, 1.0, 0.7);

	gl_Position = perspectiveMatrix * modelViewMatrix * vec4(tePosition, 1.0);
}
