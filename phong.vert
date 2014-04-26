#version 120
 
//layout (std140) uniform Matrices {
//	mat4 pvm;
//};
	  
//in vec4 position;
	   
//out vec4 color;
varying vec3 N;
varying vec3 V;
varying vec3 L;

void main()
{
    //gl_Position = pvm * position;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    // Transforming The Normal To ModelView-Space
    N = normalize(gl_NormalMatrix * gl_Normal);
    
    // Transforming The Vertex Position To ModelView-Space
    V = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // Calculating The Vector From The Vertex Position To The Light Position
    L = normalize(gl_LightSource[0].position.xyz - V);
    gl_FrontColor = gl_Color;
} 
