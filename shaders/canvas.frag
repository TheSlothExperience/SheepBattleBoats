#version 440 core

in vec2 UV;

out vec3 color;

uniform sampler2D scene;

void main(){
	color = texture(scene, UV).xyz;
}
