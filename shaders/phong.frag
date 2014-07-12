#version 440 core

in vec3 N;
in vec3 V;
in vec4 id4;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

uniform mat4 lightViews[maxLights];
uniform mat4 lightPerspectives[maxLights];
uniform sampler2D shadowMaps[maxLights];

in vec4 shadowCoords[maxLights];

uniform vec4 color;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

float linstep(float a, float b, float v) {
	return clamp((v - a) / (b - a), 0, 1);
}

float chebyshevUpperBound(float d, vec2 moments) {

	//Surface is fully lit
	if(d <= moments.x) {
		return 1.0;
	}

	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, 0.00002);

	float delta = d - moments.x;
	float p_max = variance / (variance + delta*delta);

	//Reduce light bleeding
	return linstep(0.1, 1.0, p_max);
}

void main()
{
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
	int i;

	outputColor = vec4(0.0);

	vec4 k_amb = vec4(0.2, 0.2, 0.2, 1.0) * color;
	outputColor += k_amb;
	for(i = 0; i < numLights; i++) {
		vec4 shCoord = shadowCoords[i];
		vec4 shCoordW = shCoord / shCoord.w;

		vec3 L = normalize(lightPositions[i] - V);
		//float bias = 0.005 * tan(acos(clamp(dot(N, L), 0, 1)));

		vec2 moments = texture2D(shadowMaps[i], shCoordW.xy).rg; //Distance to light
		if (shCoord.w > 0.0) {
			float visibility = chebyshevUpperBound(shCoordW.z, moments);

			vec3 R = normalize(-reflect(L,N));
			vec4 lightColor = lightColors[i];

			vec4 k_diff = visibility * color * lightColor * max(0.0, dot(L, N));
			k_diff = clamp(k_diff, 0.0, 1.0);

			vec4 k_spec = visibility * vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,E),0.0), 0.3 * 80.0);
			k_spec = clamp(k_spec, 0.0, 1.0);

			outputColor += vec4(k_diff + k_spec);
		}
	}
	pickingColor = id4;
}
