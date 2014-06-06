#version 330 core

in vec3 color;
in vec3 uv;

uniform sampler2D renderedTexture;
uniform sampler3D volumetricTexture;
uniform vec2 resolution;
uniform sampler1D transferFunction;

uniform mat4 modelViewMatrix;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

uniform int mip;

uniform int isop;
uniform float isovalue;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;

const float halfTexel = 0.5/256.0;
const float delta = 0.002;
const float epsilon = 0.0005;

//Sample the 3D texture, moved by half a texel to the center of the texel
float sampleVolumeTexture(vec3 pos) {
    return clamp(texture(volumetricTexture, pos).x + halfTexel, 0.0, 1.0 - halfTexel);
}

//Calculate the gradient using central differences, needs 6 lookups
vec3 calculateGradient(vec3 pos, float offset) {
    float vxl = texture(transferFunction, sampleVolumeTexture(pos - vec3(offset, 0, 0))).a;
    float vxr = texture(transferFunction, sampleVolumeTexture(pos + vec3(offset, 0, 0))).a;
    
    float vyl = texture(transferFunction, sampleVolumeTexture(pos - vec3(0, offset, 0))).a;
    float vyr = texture(transferFunction, sampleVolumeTexture(pos + vec3(0, offset, 0))).a;
    
    float vzl  = texture(transferFunction, sampleVolumeTexture(pos - vec3(0, 0, offset))).a;
    float vzr = texture(transferFunction, sampleVolumeTexture(pos + vec3(0, 0, offset))).a;
    
    return normalize(vec3(vxr - vxl, vyr - vyl, vzr - vzl));
}

vec4 phongShading(vec3 pos, vec4 color) {
    int i = 0;
    vec3 V = vec3(modelViewMatrix * vec4(pos, 1.0));
    vec3 E = normalize(-V);

    //Calculate the gradient at that position
    vec3 G = calculateGradient(pos, 0.01);
    
    vec4 k_amb = vec4(0.2, 0.2, 0.2, 1.0) * color;
    vec4 shadedColor = k_amb;
    
    //Iterate over the lights
    for(i = 0; i < numLights; i++) {
	vec3 L = normalize(lightPositions[i] - V);
	vec3 R = normalize(-reflect(L,G));
	vec4 lightColor = lightColors[i];	
    
	vec4 k_diff = color * lightColor * max(0.0, dot(L, G));
	k_diff = clamp(k_diff, 0.0, 1.0);

	vec4 k_spec = vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,E),0.0), 0.3 * 80.0);
	k_spec = clamp(k_spec, 0.0, 1.0);

	shadedColor += vec4(k_diff + k_spec);
    }
    
    return shadedColor;
}

vec4 rayMarchAcc(vec3 texvec, vec3 rayDir) {
    float i = 0.0;
    float alpha_acc = 0.0;
    vec4 color_acc = vec4(0.0);
    vec3 ray = normalize(rayDir);
    float len = length(rayDir);
    
    //Start the ray marching
    for(i = 0.0; i < len; i += delta) {
	//Sample the x value from the 3D tex
	float tfTexel = sampleVolumeTexture(texvec);

	//Look up the value in the TF
    	vec4 color_sample = texture(transferFunction, tfTexel);
    	float alpha_sample = 0.03 * color_sample.a;

	//Apply phong shading to the sampled color
    	color_acc += alpha_sample * phongShading(texvec, color_sample);
	
    	alpha_acc += alpha_sample;

	//Move the ray forward
    	texvec += delta * ray;
	
	//Don't oversaturate. Stop marching
    	if(alpha_acc >= 1.0) break;
    }
    return color_acc;
}

vec4 rayMarchMIP(vec3 texvec, vec3 rayDir) {
    float i = 0.0;
    float color_max = 0.0;
    vec3 vec_max = vec3(0.0);
    vec3 ray = normalize(rayDir);
    float len = length(rayDir);

    //Start the ray marching
    for(i = 0.0; i < len; i += delta) {
	//Sample the x value from the 3D tex
	float tfTexel = sampleVolumeTexture(texvec);

	if(tfTexel > color_max) {
	    color_max = tfTexel;
	    vec_max = texvec;
	}
	
	//Move the ray forward
    	texvec += delta * ray;
    }
    vec4 color_sample = texture(transferFunction, color_max);
    return color_sample;
}

vec4 rayMarchIsoSurf(vec3 texvec, vec3 rayDir) {
    float i = 0.0;
    float previous_value = 0.0;
    vec3 vec_prev = vec3(0.0);
    vec4 color_acc = vec4(0.0);
    vec3 ray = normalize(rayDir);
    float len = length(rayDir);

    int max_iterations = 100;
    //Start the ray marching
    for(i = 0.0; i < len; i += delta) {
	//Sample the x value from the 3D tex
	float tfTexel = sampleVolumeTexture(texvec);

	//If the two consecutive samples surround a zero (the isovalue)
	//then find the zero
	if((previous_value - isovalue) * (tfTexel - isovalue) < 0.0) {
	    //Isovalue in interval, use false position method to find root
	    float fs = (previous_value - isovalue);
	    float ft =  (tfTexel - isovalue);
	    vec3 s = vec_prev;
	    vec3 t = texvec;
	    vec3 r;
	    float fr;
	    int side = -1;
	    
	    for(int n = 0; n < max_iterations; n++) {
		r = (fs * t - ft * s) / (fs - ft);
		if(abs(length(t - s)) < epsilon * abs(length(t + s))) break;
		fr = sampleVolumeTexture(r);

		if(fr * ft > 0.0) {
		    t = r; ft = fr;
		    if(side == -1) fs /= 2;
		    side = -1;
		} else if (fs * fr > 0.0) {
		    s = r; fs = fr;
		    if(side == +1) ft /= 2;
		    side = +1;
		} else {
		    break;
		}
	    }
	    fr += isovalue; //renormalize
	    vec4 color_sample = texture(transferFunction, isovalue);
	    color_acc += color_sample.a * phongShading(r, color_sample);
	}
	
	//Else keep marching
	previous_value = tfTexel;
	vec_prev = texvec;
	//Move the ray forward
    	texvec += delta * ray;
    }
    return color_acc;
}

void main(){
    //Get the coords into the backface texture
    vec2 backtc = gl_FragCoord.xy / resolution.xy;
    vec4 backColor = texture(renderedTexture, backtc);

    //Get the direction vector by substracting the backface
    //from the frontface
    vec3 rayDir = backColor.xyz - color;

    if(mip != 0) { //Maximum Intensity Projection
	outputColor = rayMarchMIP(color, rayDir);
    } else if (isop != 0) { //Isosurface
	outputColor = rayMarchIsoSurf(color, rayDir);
    } else { //Normal ray marching
	outputColor = rayMarchAcc(color, rayDir);
    }

    pickingColor = vec4(-1);
}
