#version 330

layout(location = 0) in vec3 position;

uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

out vec3 texCoords;

void main () {
	texCoords = vec3(position.x, 1.0 - position.y, position.z);
	gl_Position = perspectiveMatrix * vec4((modelViewMatrix * vec4(position, 0.0)).xyz, 1.0);
}
