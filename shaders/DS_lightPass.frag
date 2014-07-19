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



float intensity(vec4 color){
    return sqrt((color.x*color.x)+(color.y*color.y)+(color.z*color.z));
}

vec3 simpleEdgeDetector(float step, vec2 center, vec4 outputColor){

    int radius = 20;
    float centerIntensity = intensity(texture2D(colorTexture,center));
    int darker = 0;
    float maxIntensity = centerIntensity;
    for(int i = -radius; i<=radius;i++){

        for(int j = -radius; j<=radius; j++){

            vec2 currentLocation = center + vec2(i*step,j*step);
            float currentIntensity = intensity(texture2D(colorTexture,currentLocation));
            if(currentIntensity <centerIntensity){
                maxIntensity = currentIntensity;
            }
        }
    }

    if((maxIntensity - centerIntensity)>0.01*radius){

        if(darker / (radius*radius) < (1-(1/radius))){
            return vec3(0.0,0.0,0.0);
        }
    }

    return outputColor.xyz;
}


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
        float diffuse = 0.0;

	for(i = 0; i < numLights; i++) {

		vec3 L = normalize(lightPositions[i] - V);
		//Blinn: Using H Vektor for cartoonish reflectance look
		vec3 H = normalize(L+E);

		diffuse += max(0.0,dot(N,L));
		float specular = max(0.0,dot(N,H));
		specular = pow(specular,0.3*80.0);

		//Stylized transformation for spekular highlights
                float edge2 = fwidth(specular);
                if(specular > 0.5-edge2 && specular < 0.5+edge2){
                        specular = smoothstep(0.5-edge2,0.5+edge2,specular);
		}else{
			specular = step(0.5,specular);
		}
		//HERE: insert materials for diffuse and specular, if finished
		outputColor += specular * vec4(1.0,1.0,1.0,1.0);
	}

	float edge = fwidth(diffuse);

	//First 3 if's: Antialiasing at transitions of colorbands
	if(diffuse > A-edge && diffuse < A+edge) diffuse = stepmix(A,B,edge,diffuse);
	else if(diffuse > B-edge && diffuse < B+edge) diffuse = stepmix(B,C,edge,diffuse);
	else if(diffuse > C-edge && diffuse < C+edge) diffuse = stepmix(C,D,edge,diffuse);
	else if(diffuse < A) diffuse = 0.0;
	else if(diffuse < B) diffuse = B;
	else if(diffuse < C) diffuse = C;
	else diffuse = D;

        outputColor += diffuse*color;

        //TRYING OUT AWESOMENSS
        float step = 1.0 / textureSize(colorTexture,0).x;
        vec2 centerColor = texture2D(colorTexture, vec2(0.5,0.5)).st;
        outputColor.xyz = simpleEdgeDetector(step,centerColor,outputColor);
        outputColor.a= 0.0;

}
