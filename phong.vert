#version 330

out vec3 N;
out vec3 V;
out vec3 L;
out float id4;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;


uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

uniform float id;

uniform vec3 lightPosition;

vec4 pack (float depth) {
    const vec4 bitSh = vec4(256 * 256 * 256,
			    256 * 256,
			    256,
			    1.0);
    const vec4 bitMsk = vec4(0,
			     1.0 / 256.0,
			     1.0 / 256.0,
			     1.0 / 256.0);
    vec4 comp = fract(depth * bitSh);
    comp -= comp.xxyz * bitMsk;
    return comp;
}

void main()
{
    gl_Position = (perspectiveMatrix * modelViewMatrix) * position;
    
    // Transforming The Normal To ModelView-Space
    N = normalize(vec3(normalMatrix * normal));
    
    // Transforming The Vertex Position To ModelView-Space
    V = vec3(modelViewMatrix * position);
    
    // Calculating The Vector From The Vertex Position To The Light Position
    L = normalize(lightPosition - V);

    //id4 = pack(id);
    id4 = id;
} 
