#version 330 core

in vec3 color;
in vec4 P;

uniform sampler2D renderedTexture;
uniform vec2 resolution;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;


void main(){

    vec2 backtc = gl_FragCoord.xy / resolution.xy;
    vec4 backColor = texture(renderedTexture, backtc);
    outputColor = vec4(color - backColor.xyz, 1.0);
    
    pickingColor = vec4(-1);
}
