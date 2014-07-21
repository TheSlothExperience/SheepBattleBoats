#version 440 core

layout(location = 0) in vec3 position;

out vec2 UV;
out vec3 vertex;

void main(){
	gl_Position =  vec4(position,1);
	UV = (position.xy+vec2(1,1))/2.0;
        vertex = position;
}
