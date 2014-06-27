#version 440 core

layout(vertices = 4) out;

in vec3 vPosition[];
out vec3 tcPosition[];

out float mipLevel[];

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform float heightScale;

#define ID gl_InvocationID

float level(vec3 vec) {
	float d = length(vec) * 100.0/heightScale;
	float factor = 150.0 * (1.0/(d*d));
	float lod = pow(2.0, floor(factor));
	lod = clamp(lod, 2, 64);
	return lod;
}

vec3 vecToMidpoint(vec3 a, vec3 b) {
	return a + (b - a) / 2.0;
}

void main()
{
	tcPosition[ID] = vPosition[ID];

	vec4 v0 = modelViewMatrix * vec4(vPosition[0], 1.0);
	vec4 v1 = modelViewMatrix * vec4(vPosition[1], 1.0);
	vec4 v2 = modelViewMatrix * vec4(vPosition[2], 1.0);
	vec4 v3 = modelViewMatrix * vec4(vPosition[3], 1.0);

	float lod0 = level(vecToMidpoint(v0.xyz, v2.xyz));
	float lod1 = level(vecToMidpoint(v0.xyz, v1.xyz));
	float lod2 = level(vecToMidpoint(v1.xyz, v3.xyz));
	float lod3 = level(vecToMidpoint(v2.xyz, v3.xyz));

	//Write the mipLevel at this id
	switch (ID) {
	case 0: mipLevel[ID] = log2(level(v0.xyz)); break;
	case 1: mipLevel[ID] = log2(level(v1.xyz)); break;
	case 2: mipLevel[ID] = log2(level(v2.xyz)); break;
	case 3: mipLevel[ID] = log2(level(v3.xyz)); break;
	}

	if(ID == 0) {
		float maxLod = max(max(lod1, lod3), max(lod0, lod2));
		gl_TessLevelInner[0] = maxLod;
		gl_TessLevelInner[1] = maxLod;

		//CW starting on the left
		gl_TessLevelOuter[0] = lod0;
		gl_TessLevelOuter[1] = lod1;
		gl_TessLevelOuter[2] = lod2;
		gl_TessLevelOuter[3] = lod3;
	}
}
