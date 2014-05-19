#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D pickingTexture;

uniform float selected;
uniform vec4 activeColor;

void main(){
    float x = UV.x;
    float y = UV.y;

    float edgeEps = 0.01;
    //Check if selected and if the pix is in the border -> mark yellow!
    if(selected > 0.5 && (x < 0.01 || x > 0.99 || y < 0.01 || y > 0.99)) {
    	color = vec3(1.0, 1.0, 0.0);
    } else {
	//Sample the picking color and compare it to the current color
	vec4 pickColor = texture(pickingTexture, UV);
	if(pickColor == activeColor) {
	    //Sample around the pixel to see if the color changes
	    vec4 left = texture(pickingTexture, UV - vec2(edgeEps, 0.0));
	    vec4 right = texture(pickingTexture, UV + vec2(edgeEps, 0.0));
	    vec4 up = texture(pickingTexture, UV + vec2(0.0, edgeEps));
	    vec4 down = texture(pickingTexture, UV - vec2(0.0, edgeEps));
	    //If diffent, then edge! Mark that thang!!!
	    if(left != pickColor || right != pickColor || up != pickColor || down != pickColor) {
		color = vec3(1.0, 1.0, 0.0); 
	    } else {
		color = texture(renderedTexture, UV).xyz;
	    }
	} else {
	    color = texture(renderedTexture, UV).xyz;
	}
    }
}
