#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aValue;

out VS_OUT 
{
	vec3 pos;
	float value;
} vs_out;

void main()
{
	vs_out.pos = aPos;
	vs_out.value  = aValue;
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 pos;
	in float value;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 planes[8];

out vec3 Normal;
out vec3 FragPos;
out float value;

void main() 
{
	Normal = normalize(mat3(transpose(inverse(model))) * cross(gs_in[1].pos -  gs_in[0].pos, gs_in[2].pos -  gs_in[1].pos));
    for (int i = 0; i < 3; ++i) {
		FragPos = vec3(model * vec4(gs_in[i].pos, 1.0));
		gl_Position = projection * view * vec4(FragPos, 1.0);
		value  = gs_in[i].value;
        EmitVertex();
    }
    EndPrimitive();
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

in vec3 FragPos;  
in vec3 Normal; 
in float value;

uniform sampler1D Texture;
uniform int lightIsOn;
uniform float minValue;
uniform float maxValue;
uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 normal, vec3 matColor)
{
	if(lightIsOn == 0)
	{
		return matColor;
	}
	vec3 ambient = light.ambient * matColor;	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * matColor);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * (spec * 0.3);  

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = gl_FrontFacing ? Normal : -Normal;
	vec3 color = vec3(texture(Texture, (value-minValue) / (maxValue-minValue)));
	FragColor = vec4(calculateLightResult(normal, color), 1.0);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
} 
#endif
