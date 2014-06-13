#version 440 core

layout(vertices = 4) out;

in vec3 vPosition[];
out vec3 tcPosition[];

const float tessLevelInner = 4;
const float tessLevelOuter = 4;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

#define ID gl_InvocationID

float level(vec3 vec) {
	float d = length(vec);
	float factor = 150.0 * (1.0/(d*d));
	float lod = pow(2.0, floor(factor));
	lod = clamp(lod, 1, 64);
	return lod;
}

void main()
{
	tcPosition[ID] = vPosition[ID];

	float lod = level((modelViewMatrix * vec4(vPosition[ID], 1.0)).xyz);
	if(ID == 0) {
		gl_TessLevelInner[0] = lod;
		gl_TessLevelInner[1] = lod;
		gl_TessLevelOuter[0] = lod;
		gl_TessLevelOuter[1] = lod;
		gl_TessLevelOuter[2] = lod;
		gl_TessLevelOuter[3] = lod;
	}
}
