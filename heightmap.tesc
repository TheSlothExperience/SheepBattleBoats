#version 440 core

layout(vertices = 4) out;

in vec3 vPosition[];
out vec3 tcPosition[];

const float tessLevelInner = 4;
const float tessLevelOuter = 4;

#define ID gl_InvocationID

void main()
{
	tcPosition[ID] = vPosition[ID];

	if(ID == 0) {
        gl_TessLevelInner[0] = tessLevelInner;
        gl_TessLevelInner[1] = tessLevelInner;
        gl_TessLevelOuter[0] = tessLevelOuter;
        gl_TessLevelOuter[1] = tessLevelOuter;
        gl_TessLevelOuter[2] = tessLevelOuter;
        gl_TessLevelOuter[3] = tessLevelOuter;
	}
}
