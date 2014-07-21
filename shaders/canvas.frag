#version 440 core

layout(location = 0) in vec3 position;

in vec2 UV;
in vec3 vertex;

out vec4 color;

uniform sampler2D scene;
uniform sampler2D blurredIntensity;
uniform samplerCube cubemap;


void main(){
        color = vec4(texture(scene, UV).xyz+texture(blurredIntensity,UV).xyz,1.0);
}
