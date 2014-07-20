uniform sampler2D scene;
uniform sampler2D blurIntensity;
uniform vec2 pixelSize;

void main(){

    vec2 pos=gl_FragCoord.xy*pixelSize;
    gl_FragColor = vec4(texture2D(scene,pos).rgb+texture2D(blurIntensity,pos).rgb,1.0);

}
