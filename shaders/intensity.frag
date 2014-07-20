uniform sampler2D scene;

void main(){

	vec2 size = textureSize(scene, 0);
    vec2 pos = gl_FragCoord.xy*(1.0 / size);
    float intensity = dot(texture2D(scene,pos).xyz,vec3(0.333,0.333,0.333));
    if(intensity > 0.7){
        gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    }else{
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    }
}
