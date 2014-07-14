#version 440 core

in vec2 UV;
uniform sampler2D moments;

layout(location = 0) out vec4 blurred;

void main() {
	vec4 sum = vec4(0.0);
	float delta = 1.0 / textureSize(moments, 0).x;

	sum += texture(moments, vec2(UV.x - 4.0 * delta, UV.y)) * 0.05;
	sum += texture(moments, vec2(UV.x - 3.0 * delta, UV.y)) * 0.09;
	sum += texture(moments, vec2(UV.x - 2.0 * delta, UV.y)) * 0.12;
	sum += texture(moments, vec2(UV.x - delta, UV.y)) * 0.155;
	sum += texture(moments, vec2(UV.x, UV.y)) * 0.17;
	sum += texture(moments, vec2(UV.x + delta, UV.y)) * 0.155;
	sum += texture(moments, vec2(UV.x + 2.0 * delta, UV.y)) * 0.12;
	sum += texture(moments, vec2(UV.x + 3.0 * delta, UV.y)) * 0.09;
	sum += texture(moments, vec2(UV.x + 4.0 * delta, UV.y)) * 0.05;

	blurred = sum;
}
