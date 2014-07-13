#version 330 core

in vec2 UV;

out vec4 outputColor;

uniform sampler2D posTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D depthTexture2;

uniform float selected;
uniform vec4 activeColor;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

void main(){

    vec4 color=vec4(texture(colorTexture,UV));
    vec3 V=vec3(texture(posTexture,UV));
    vec3 N=vec3(texture(normalTexture,UV));

    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    int i;

    vec4 k_amb = vec4(0.2, 0.2, 0.2, 1.0) * color;
    outputColor = k_amb;
    for(i = 0; i < numLights; i++) {

            vec3 L = normalize(lightPositions[i] - V);
            vec3 R = normalize(-reflect(L,N));

            vec4 lightColor = lightColors[i];

            vec4 k_diff = color * lightColor * max(0.0, dot(L, N));
            k_diff = clamp(k_diff, 0.0, 1.0);

            vec4 k_spec = vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,E),0.0), 0.3 * 80.0);
            k_spec = clamp(k_spec, 0.0, 1.0);

            outputColor += vec4(k_diff + k_spec);
    }
}
