#version 440 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

in vec3 color;

void main()
{
	//outputColor = vec4(1.0, 0.5, 0.5, 1.0);
	outputColor = vec4(color, 1.0);
    pickingColor = vec4(1.0);
}
