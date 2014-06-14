#version 440 core

layout(quads) in;
              
in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;

out vec3 color;

uniform sampler2D heightMapTexture;
                 
uniform mat4 perspectiveMatrix;
uniform mat4 modelViewMatrix;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	//mix() does linear interpolation
	//here we are just doing some lerping
	vec3 s = mix(tcPosition[0], tcPosition[1], u);
	vec3 t = mix(tcPosition[2], tcPosition[3], u);

	tePosition = mix(s, t, v);
	float height = textureLod(heightMapTexture, (tePosition.xz + 25.0) / 50.0, 1).r;
	height *= 100.0;
	
	color = vec3(height/5.0);
	
	gl_Position = perspectiveMatrix * modelViewMatrix * vec4(tePosition.x, height, tePosition.z, 1.0);
}
