#version 440 core

in vec2 vTexCoords;
in float  vAlpha;

uniform sampler2D sprite;

void main(void)
{
	gl_FragColor = texture(sprite, vTexCoords);
	if(gl_FragColor.a > 0.0) {
		gl_FragDepth = gl_FragCoord.z;
	} else {
		gl_FragDepth = 1.0;
	}
}
