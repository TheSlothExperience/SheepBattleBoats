#version 330

in vec3 N;
in vec3 V;
in  vec3 L;

out vec4 outputColor;
void main()
{

    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L,N));
    
    vec4 k_amb = vec4(0.1, 0.1, 0.1, 1.0);
    
    //vec4 k_diff = gl_FrontLightProduct[0].diffuse * max(0.0, dot(L, N));
    vec4 k_diff = color * max(0.0, dot(L, N));
    k_diff = clamp(k_diff, 0.0, 1.0);

    vec4 k_spec = vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R,-V),0.0), 80.0);
    k_spec = clamp(k_spec, 0.0, 1.0);

    /* gl_FragColor =  k_amb + k_diff + k_spec; */
    //outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    outputColor = vec4(vec3(dot(R,E)), 1.0);
    //outputColor = k_amb + k_diff + k_spec;
}
