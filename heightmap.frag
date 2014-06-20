#version 440 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

in vec3 color;
in vec3 position;

const int maxFactures = 12;
uniform sampler2D factures[maxFactures];
uniform int numFactures;

void main()
{
	outputColor = vec4(color, 1.0);
	for(int i = 0; i < numFactures; i++) {
		outputColor = texture(factures[i], position.xz);
	}
    pickingColor = vec4(1.0);
}
