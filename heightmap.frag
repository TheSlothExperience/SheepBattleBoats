#version 440 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

in vec3 tc;

in vec3 gTriDistance;
in vec4 gPatchDistance;
in vec3 gNormal;
in vec3 V;

const int maxFactures = 12;
uniform sampler2D factures[maxFactures];
uniform int numFactures;
uniform float maxHeight;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

uniform int showMeshp;

const vec4 meshColor = vec4(0.0, 1.0, 0.0, 1.0);

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

vec4 phong(vec4 color) {
	vec3 E = normalize(-V);

	vec4 k_amb = vec4(0.2, 0.2, 0.2, 1.0) * color;
	vec4 outC = k_amb;
	int i;
	for(i = 0; i < numLights; i++) {
		vec3 L = normalize(lightPositions[i] - V);
		vec3 R = normalize(-reflect(L,gNormal));
		vec4 lightColor = lightColors[i];

		vec4 k_diff = color * lightColor * max(0.0, dot(L, gNormal));
		k_diff = clamp(k_diff, 0.0, 1.0);

		vec4 k_spec = vec4(0.7, 0.7, 0.7, 0.7) * pow(max(dot(R,E),0.0), 0.3 * 10.0);
		k_spec = clamp(k_spec, 0.0, 1.0);

		outC += vec4(k_diff + k_spec);
	}
	return outC;
}

void main()
{
	outputColor = vec4(gNormal, 1.0);

	float heightStep = maxHeight / numFactures;
	for(int i = 0; i < numFactures; i++) {
		//Sample two levels in order to smooth them
		vec4 sampledColor = texture(factures[i], tc.xz);
		vec4 sampledColorNext = texture(factures[min(numFactures - 1, i + 1)], tc.xz);
		if(tc.y > i * heightStep) {
			//Calculate smooth interpolation between facture texes depending on height.
			float lambda = smoothstep((i + 1) * heightStep, i * heightStep, tc.y);
			vec4 terrain = mix(sampledColorNext, sampledColor, lambda);
			outputColor = phong(terrain);
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
