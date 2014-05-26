#version 330 core

in vec3 color;

uniform sampler2D renderedTexture;
uniform vec2 resolution;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;


void main(){
    //Get the coords into the backface texture
    vec2 backtc = gl_FragCoord.xy / resolution.xy;
    vec4 backColor = texture(renderedTexture, backtc);

    //Get the direction vector by substracting the backface
    //from the frontface
    outputColor = vec4(color - backColor.xyz, 1.0);
    
    pickingColor = vec4(-1);
}
