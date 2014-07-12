#version 440 core

in vec4 P;

layout(location = 0) out vec4 moments;

void main()
{
	float depth = P.z / P.w ;
	depth = depth * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

	float moment1 = depth;
	float moment2 = depth * depth;

	// Adjusting moments (this is sort of bias per pixel) using derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25 * (dx*dx + dy*dy);


	moments = vec4(moment1, moment2, 0.0, 0.0);
}
