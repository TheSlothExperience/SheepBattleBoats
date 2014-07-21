#version 440 core

in vec2 UV;
uniform sampler2D moments;

layout(location = 0) out vec4 blurred;

void main() {
	vec4 sum = vec4(0.0);
        float delta = 1.2 / textureSize(moments, 0).x;

        sum += texture(moments, vec2(UV.x - 11.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 10.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 9.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 8.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 7.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 6.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 5.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 4.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 3.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - 2.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x - delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 2.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 3.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 4.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 5.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 6.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 7.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 8.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 9.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 10.0 * delta, UV.y)) * 0.076923077;
        sum += texture(moments, vec2(UV.x + 11.0 * delta, UV.y)) * 0.076923077;

	blurred = sum;
}
