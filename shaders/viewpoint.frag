#version 440

in vec3 V;
in vec3 color;

layout(location = 0) out vec4 outputColor;
void main()
{
    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    int i;
    outputColor += vec4(color,1.0);
}
