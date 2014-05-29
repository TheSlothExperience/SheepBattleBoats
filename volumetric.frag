#version 330 core

in vec3 color;
in vec3 uv;

uniform sampler2D renderedTexture;
uniform sampler3D volumetricTexture;
uniform vec2 resolution;
uniform sampler1D transferFunction;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;


void main(){
    //Get the coords into the backface texture
    vec2 backtc = gl_FragCoord.xy / resolution.xy;
    vec4 backColor = texture(renderedTexture, backtc);

    //Get the direction vector by substracting the backface
    //from the frontface
    vec3 rayDir = backColor.xyz - color;
    float len = length(rayDir);
    
    float delta = 0.002;
    float i = 0.0;
    float alpha_acc = 0.0;
    vec4 color_acc = vec4(0.0);
    vec3 texvec = color;
    for(i = 0.0; i < 1.0; i += delta) {
        float vol_sample = texture(volumetricTexture, texvec).x;
    	vec4 color_sample = texture(transferFunction, vol_sample);
    	float alpha_sample = 0.03 * color_sample.a;
    	color_acc += alpha_sample * color_sample;
    	alpha_acc += alpha_sample;
    	texvec += delta * rayDir;
    	if(alpha_acc >= 1.0) break;
    }
    outputColor = color_acc;
    pickingColor = vec4(-1);
}
