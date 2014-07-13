#version 330

in vec3 N;
in vec3 V;
in vec2 texCoord0;
in vec4 id4;



uniform vec4 color;

layout (location = 0) out vec3 worldPosOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 depthOut;
layout (location = 4) out vec3 textCoordOut;
void main(){

        worldPosOut = V;
        diffuseOut = color.xyz;
        normalOut = normalize(N);
        depthOut=vec3(gl_FragCoord.z);
        textCoordOut=vec3(texCoord0,0.0);
//        TexCoordOut = vec3(texCoord0, 0.0);
//        TexCoordOut = vec3(1.0, 1.0, 0.0);

}
