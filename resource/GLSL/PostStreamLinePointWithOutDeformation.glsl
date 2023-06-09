#version 450 core

#ifdef vertex_shader
layout(location = 0) in vec3 aPos;
layout(location = 1) in float aValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int PointSize;

out float value;

//光照部分
out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	value = aValue;
	gl_PointSize = PointSize;
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

in float value;
uniform sampler1D Texture;
uniform int lightIsOn;
uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)

uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 norm, vec3 color, float AmbientOcclusion, float z)
{
    if(lightIsOn == 0) return color;

    vec3 viewDir = normalize(viewPos - FragPos);
    // if (dot(norm, viewDir) < 0.0) norm = -norm;
    norm = normalize(viewDir);

    vec3 ambient = light.ambient * color;//ambi
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color * AmbientOcclusion * z;//diffu
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * spec * 0.3 * AmbientOcclusion * z;//specu
    return ambient + diffuse + specular;
}

void main()
{
    vec3 N;
    N.xy = gl_PointCoord * 2.0f - 1.0f;
    
    float mag = dot(N.xy,N.xy);
    if(mag > 0.98f) discard;
    N.z = sqrt(1.0f - mag);
    vec3 color = vec3(texture(Texture, (isEquivariance == 0 ? log2(value/minValue) / log2(maxValue/minValue) : (value-minValue) / (maxValue-minValue))));
	FragColor.rgb = pow(calculateLightResult(N, color, 1.0, N.z), vec3(1.0 / 2.2));
    FragColor.a = 1.0;
} 

#endif