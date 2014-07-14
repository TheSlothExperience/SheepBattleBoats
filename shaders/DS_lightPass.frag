#version 330 core

in vec2 UV;

out vec4 outputColor;

uniform sampler2D posTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D depthTexture2;

uniform float selected;
uniform vec4 activeColor;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

float stepmix(float edge0, float edge1, float E, float x){
    float T = clamp(0.5*(x-edge0+E)/E,0.0,1.0);
    return mix(edge0,edge1,T);
}

void main(){

    vec4 color=vec4(texture(colorTexture,UV));
    vec3 V=vec3(texture(posTexture,UV));
    vec3 N=vec3(texture(normalTexture,UV));

    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    int i;
	const float A = 0.1;
	const float B = 0.33;
	const float C = 0.66;
	const float D = 1.0;
	outputColor = vec4(0.0);

	for(i = 0; i < numLights; i++) {

		vec3 L = normalize(lightPositions[i] - V);
		//Blinn: Using H Vektor for cartoonish reflectance look
		vec3 H = normalize(L+E);

		vec4 lightColor = lightColors[i];

		float diffuse = max(0.0,dot(N,L));
		float specular = max(0.0,dot(N,H));
		specular = pow(specular,0.3*80.0);

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
			specular = smoothstep(0.5-edge,0.5+edge,specular);
		}else{
			specular = step(0.5,specular);
		}
		//HERE: insert materials for diffuse and specular, if finished
		vec4 color = diffuse*color*lightColor+specular*vec4(1.0,1.0,1.0,1.0);
		outputColor += color;
	}
}
