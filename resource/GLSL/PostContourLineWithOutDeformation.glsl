#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float value;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	value  = aValue;
}
#endif



#ifdef fragment_shader


out vec4 FragColor;

in float value;

uniform sampler1D Texture;
uniform float minValue;
uniform float maxValue;

void main()
{
//	float distance = length(light.position - FragPos);
//    float attenuation = 1.0 / (distance * distance);
	vec3 color = vec3(texture(Texture, (value-minValue) / (maxValue-minValue)));
    FragColor.rgb = pow(color, vec3(1.0/2.2));
} 

#endif


