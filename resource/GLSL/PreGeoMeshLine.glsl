#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aMaterial;

uniform mat4 pvm;

out vec3 material;

void main()
{
	gl_Position = pvm * vec4(aPos, 1.0);
    material = aMaterial;
}
#endif



#ifdef fragment_shader

in vec3 material;
out vec4 FragColor;

void main()
{	
    FragColor = vec4(material,1);
} 

#endif


