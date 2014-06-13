#version 440 core

layout(isolines) in;
              
in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;

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
	gl_Position = perspectiveMatrix * modelViewMatrix * vec4(tePosition, 1.0);
}
