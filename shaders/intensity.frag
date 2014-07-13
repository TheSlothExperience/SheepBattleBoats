uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

uniform vec4 color;


void main(void)
{
    vec2 pos = gl_FragCoord.xy;
    float intensity = dot(texture2D(sampler,pos).xyz,vec3(0.33,0.33,0.33));
    if(intensity>0.8)
        gl_FragColor = color;
    else
        gl_FragColor= vec4(0.0,0.0,0.0,1.0);
}
