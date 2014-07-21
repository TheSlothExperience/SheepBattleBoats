#version 330

in vec3 texCoords;

uniform samplerCube cubeTex;

out vec4 outputColor;

void main() {
	outputColor = vec4(texture(cubeTex, texCoords).xyz, 1.0);
}
