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

float stepmix(float edge0, float edge1, float E, float x){
	float T = clamp(0.5*(x-edge0+E)/E,0.0,1.0);
	return mix(edge0,edge1,T);
}

void main()
{
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
	int i;
	const float A = 0.1;
	const float B = 0.33;
	const float C = 0.66;
	const float D = 1.0;

	vec4 k_amb = vec4(0.1, 0.1, 0.1, 1.0) * color;
	outputColor = k_amb;
	for(i = 0; i < numLights; i++) {

		vec3 L = normalize(lightPositions[i] - V);

		vec4 shCoord = shadowCoords[i];
		vec4 shCoordW = shCoord / shCoord.w;

		float bias = 0.005 * tan(acos(clamp(dot(N, L), 0, 1)));

		float shadowD = texture2D(shadowMaps[i], shCoordW.xy).z; //Distance to light
		float shadowFactor;
		if(shadowD > shCoordW.z - 0.001) {
			shadowFactor = 1.0;
		} else {
			shadowFactor = 0.0;
		}
		//Blinn: Using H Vektor for cartoonish reflectance look
		vec3 H = normalize(L+E);

		vec4 lightColor = lightColors[i];

		float diffuse = max(0.0,dot(N,L));
		float specular = max(0.0,dot(N,H));
		specular = pow(specular,0.3 * 80.0);

		float edge = fwidth(diffuse);

		//First 3 if's: Antialiasing at transitions of colorbands
		if(diffuse > A-edge && diffuse < A+edge) diffuse = stepmix(A,B,edge,diffuse);
		else if(diffuse > B-edge && diffuse < B+edge) diffuse = stepmix(B,C,edge,diffuse);
		else if(diffuse > C-edge && diffuse < C+edge) diffuse = stepmix(C,D,edge,diffuse);
		else if(diffuse < A) diffuse = 0.0;
		else if(diffuse < B) diffuse = B;
		else if(diffuse < C) diffuse = C;
		else diffuse = D;

		//Stylized transformation for spekular highlights
		edge = fwidth(specular);
		if(specular > 0.5-edge && specular < 0.5+edge){
			specular = smoothstep(0.5-edge, 0.5 + edge, specular);
		} else {
			specular = step(0.5,specular);
		}
		//HERE: insert materials for diffuse and specular, if finished
		vec4 shadedColor = diffuse * color * lightColor + specular * vec4(1.0,1.0,1.0,1.0);
		outputColor += shadowFactor * shadedColor;
	}

	pickingColor = id4;
}
