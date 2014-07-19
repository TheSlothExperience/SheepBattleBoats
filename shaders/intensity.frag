uniform sampler2D scene;
uniform vec2 pixelSize;

void main(){

    vec2 pos = gl_FragCoord.xy*pixelSize;
    float intensity = dot(texture2D(scene,pos).xyz,vec3(0.333,0.333,0.333));
    if(intensity > 0.8){
        gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    }else{
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    }
}
