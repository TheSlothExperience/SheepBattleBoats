#version 440 core

in vec2 UV;
uniform sampler2D moments;

layout(location = 0) out vec4 blurred;

void main() {
	vec4 sum = vec4(0.0);
        float delta = 1.0 / textureSize(moments, 0).y;

        sum += texture(moments, vec2(UV.x, UV.y - 7.0 * delta)) * 8.740619478223898e-3;
        sum += texture(moments, vec2(UV.x, UV.y - 6.0 * delta)) * 1.799696779537199e-2;
        sum += texture(moments, vec2(UV.x, UV.y - 5.0 * delta)) * 3.315900749418044e-2;
        sum += texture(moments, vec2(UV.x, UV.y - 4.0 * delta)) * 5.4669960970960246e-2;
        sum += texture(moments, vec2(UV.x, UV.y - 3.0 * delta)) * 8.065681386774479e-2;
        sum += texture(moments, vec2(UV.x, UV.y - 2.0 * delta)) * 0.1064825440062698;
        sum += texture(moments, vec2(UV.x, UV.y - delta)) * 0.1257942621502312;
        sum += texture(moments, vec2(UV.x, UV.y)) * 0.1329806046506519;
        sum += texture(moments, vec2(UV.x, UV.y + delta)) * 0.1257942621502312;
        sum += texture(moments, vec2(UV.x, UV.y + 2.0 * delta)) * 0.1064825440062698;
        sum += texture(moments, vec2(UV.x, UV.y + 3.0 * delta)) * 8.065681386774479e-2;
        sum += texture(moments, vec2(UV.x, UV.y + 4.0 * delta)) * 5.4669960970960246e-2;
        sum += texture(moments, vec2(UV.x, UV.y + 5.0 * delta)) * 3.315900749418044e-2;
        sum += texture(moments, vec2(UV.x, UV.y + 6.0 * delta)) * 1.799696779537199e-2;
        sum += texture(moments, vec2(UV.x, UV.y + 7.0 * delta)) * 8.740619478223898e-3;

	blurred = sum;
}
