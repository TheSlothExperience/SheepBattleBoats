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


uniform vec4 color;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

float stepmix(float edge0, float edge1, float E, float x){
	float T = clamp(0.5*(x-edge0+E)/E,0.0,1.0);
	return mix(edge0,edge1,T);
}

void main()
{

    //TODO: Include Sobel Operator to detect edges on normal tex and depth tex
    //then finished
        vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
        int i;
        const float A = 0.1;
        const float B = 0.33;
        const float C = 0.66;
        const float D = 1.0;

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
               outputColor +=specular*vec4(1.0,1.0,1.0,1.0) ;

        }

        //diffuse /= numLights;

        float edge = fwidth(diffuse);

        //First 3 if's: Antialiasing at transitions of colorbands
        if(diffuse > A-edge && diffuse < A+edge) diffuse = stepmix(A,B,edge,diffuse);
        else if(diffuse > B-edge && diffuse < B+edge) diffuse = stepmix(B,C,edge,diffuse);
        else if(diffuse > C-edge && diffuse < C+edge) diffuse = stepmix(C,D,edge,diffuse);
        else if(diffuse < A) diffuse = 0.0;
        else if(diffuse < B) diffuse = B;
        else if(diffuse < C) diffuse = C;
        else diffuse = D;

        outputColor+= diffuse*color;

        pickingColor = id4;

}
