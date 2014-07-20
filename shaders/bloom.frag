uniform sampler2D scene;
uniform sampler2D blurIntensity;

void main(){

    vec2 size = textureSize(scene, 0);
    vec2 pos = gl_FragCoord.xy*(1.0 / size);
    gl_FragColor = vec4(texture2D(scene,pos).rgb+texture2D(blurIntensity,pos).rgb,1.0);

}
