#version 440 core

in vec2 UV;

out vec3 color;

uniform sampler2D scene;
uniform sampler2D blurredIntensity;


void main(){
        gl_FragColor = vec4(texture(scene, UV).xyz+texture(blurredIntensity,UV).xyz,1.0);
}
