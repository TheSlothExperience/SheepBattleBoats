#version 440 core

layout(location = 0) in vec3 position;

in vec2 UV;
in vec3 vertex;

out vec4 color;

uniform sampler2D scene;
uniform sampler2D blurredIntensity;
uniform int bitify;
uniform int scopify;
uniform int crossify;

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

    if(crossify==1){
        float lum = length(texture(scene,coord).xyz);
        vec3 tc=vec3(1.0,1.0,1.0);

        if (lum < 1.0) {
              if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)
                tc = vec3(0.0, 0.0, 0.0);
            }
        if (lum < 0.7){
              if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)
                tc = vec3(0.0, 0.0, 0.0);
            }
        if (lum < 0.5) {
              if (mod(gl_FragCoord.x + gl_FragCoord.y - 5.0, 10.0) == 0.0)
                tc = vec3(0.0, 0.0, 0.0);
            }
        if (lum < 0.3){
              if (mod(gl_FragCoord.x - gl_FragCoord.y - 5.0, 10.0) == 0.0)
                tc = vec3(0.0, 0.0, 0.0);
            }
        color += vec4(tc,1.0);
    }

}
