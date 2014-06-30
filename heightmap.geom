#version 440 core

uniform mat4 modelViewMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec4 tePatchDistance[3];
in vec3 teTc[3];
in vec3 teNormal[3];

out vec3 tc;
out vec4 gPatchDistance;
out vec3 gTriDistance;
out vec3 gNormal;

void main()
{
	//Use the triDistance as barycentric coords to determine
	//distance to border in the fragment shader
    gPatchDistance = tePatchDistance[0];
    tc = teTc[0];
    gNormal = teNormal[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gPatchDistance = tePatchDistance[1];
    tc = teTc[1];
    gNormal = teNormal[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gPatchDistance = tePatchDistance[2];
    tc = teTc[2];
    gNormal = teNormal[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
