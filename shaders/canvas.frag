#version 440 core

layout(location = 0) in vec3 position;

in vec2 UV;
in vec3 vertex;

out vec4 color;

uniform sampler2D scene;
uniform sampler2D blurredIntensity;
uniform int bitify;
uniform int scopify;

void main(){

    vec2 coord = UV;

    if(bitify == 1){
        vec2 size = textureSize(scene,0);
        float dx = 15.0 * (1.0 / size.x);
        float dy = 10.0 * (1.0 / size.y);
        coord = vec2(dx*floor(UV.x/dx),dy*floor(UV.y/dy));
    }


    color = vec4(texture(scene, coord).xyz+texture(blurredIntensity,coord).xyz,1.0);

    if(scopify==1){
        float dist = distance(coord.xy,vec2(0.5,0.5));
        color*= smoothstep(0.45,0.38,dist);
    }


}
