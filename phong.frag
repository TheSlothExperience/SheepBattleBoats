#version 330

in vec3 N;
in vec3 V;
in vec3 L;
in vec4 id4;

layout(location = 0) out vec4 outputColor;
layout(location = 1) out vec4 pickingColor;
void main()
{

    vec4 color = vec4(0.8, 0.3, 0.0, 1.0);
    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L,N));
    
    vec4 k_amb = vec4(0.2, 0.2, 0.2, 1.0) * color;
    
    vec4 k_diff = color * max(0.0, dot(L, N));
    k_diff = clamp(k_diff, 0.0, 1.0);

    vec4 k_spec = vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,E),0.0), 0.3 * 80.0);
    k_spec = clamp(k_spec, 0.0, 1.0);

    outputColor = vec4(k_amb + k_diff + k_spec);
    pickingColor = id4;
}
