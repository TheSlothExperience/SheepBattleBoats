#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D pickingTexture;

uniform float selected;

void main(){
    float x = UV.x;
    float y = UV.y;

    //Check if selected and if the pix is in the border -> mark yellow!
    if(selected > 0.5 && (x < 0.01 || x > 0.99 || y < 0.01 || y > 0.99)) {
    	color = vec3(1.0, 1.0, 0.0);
    } else {
    	color = texture(renderedTexture, UV).xyz;
    }
}
