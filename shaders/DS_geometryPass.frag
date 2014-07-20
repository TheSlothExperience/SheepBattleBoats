#version 330

in vec3 N;
in vec3 V;
in vec2 texCoord0;
in vec4 id4;



uniform vec4 color;

layout (location = 0) out vec4 worldPosOut;
layout (location = 1) out vec4 diffuseOut;
layout (location = 2) out vec4 normalOut;
layout (location = 3) out vec4 depthOut;

void main(){
	worldPosOut = vec4(V, 1.0);
	diffuseOut = color;
	normalOut = vec4(normalize(N), 1.0);
	depthOut = vec4(gl_FragCoord.z);
}
