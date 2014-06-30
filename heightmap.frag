#version 440 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

in vec3 tc;

in vec3 gTriDistance;
in vec4 gPatchDistance;
in vec3 gNormal;

const int maxFactures = 12;
uniform sampler2D factures[maxFactures];
uniform int numFactures;
uniform float maxHeight;

uniform int showMeshp;

const vec4 meshColor = vec4(0.0, 1.0, 0.0, 1.0);

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
	outputColor = vec4(gNormal, 1.0);

	float heightStep = maxHeight / numFactures;
	for(int i = 0; i < numFactures; i++) {
		vec4 sampledColor = texture(factures[i], tc.xz);
		if(tc.y > i * heightStep) {
			outputColor = sampledColor;
		}
	}

	//Draw the wireframe
	if(showMeshp != 0) {
		//Distance to the tessellated triangle border
        float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
        //Distance to patch border
        float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
        d1 = 1 - amplify(d1, 80, -0.01);
        d2 = amplify(d2, 80, -0.2);
        outputColor = d2 * outputColor + d1 * d2 * meshColor;
	}

    pickingColor = vec4(1.0);
}
