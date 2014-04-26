#version 120
 
//layout (std140) uniform Matrices {
//	mat4 pvm;
//};
	  
//in vec4 position;
	   
//out vec4 color;
	    
void main()
{
	//gl_Position = pvm * position;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 
