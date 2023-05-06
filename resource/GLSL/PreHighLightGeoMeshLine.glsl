#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(aPos, 1.0);
    gl_Position.z = gl_Position.z - 0.001;
}
#endif



#ifdef fragment_shader

uniform vec3 showColor;
out vec4 FragColor;

void main()
{	
    FragColor = vec4(showColor,1);
} 

#endif


