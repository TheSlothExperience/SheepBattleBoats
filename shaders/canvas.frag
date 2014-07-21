#version 440 core

layout(location = 0) in vec3 position;

in vec2 UV;

out vec4 color;

uniform sampler2D scene;
uniform sampler2D blurredIntensity;
uniform samplerCube cubemap;


void main(){
        color = textureCube(cubemap,position);
        //vec4(texture(scene, UV).xyz+texture(blurredIntensity,UV).xyz,1.0)
}
