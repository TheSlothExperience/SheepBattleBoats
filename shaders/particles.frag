#version 440 core

in vec2 vTexCoords;
in float  vAlpha;

uniform sampler2D sprite;

void main(void)
{
	vec4 outputColor = texture(sprite, vTexCoords);
	if(outputColor.a > 0.0) {
		gl_FragColor = outputColor;
		gl_FragDepth = gl_FragCoord.z;
	} else {
		gl_FragDepth = 1.0;
		discard;
	}
}
