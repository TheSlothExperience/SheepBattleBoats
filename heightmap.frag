#version 440 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

in vec3 color;
in vec3 position;

const int maxFactures = 12;
uniform sampler2D factures[maxFactures];
uniform int numFactures;
uniform float maxHeight;

void main()
{
	outputColor = vec4(color, 1.0);
	float heightStep = maxHeight / numFactures;
	for(int i = 0; i < numFactures; i++) {
		vec4 sampledColor = texture(factures[i], position.xz);
		if(position.y > i * heightStep) {
			outputColor = sampledColor;
		}
	}
    pickingColor = vec4(1.0);
}
