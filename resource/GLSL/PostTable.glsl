#version 330 core

#ifdef vertex_shader
layout (location = 0) in vec2 aPos;
layout(location = 1) in float aTexCoord;

out float TexCoord;

void main()
{
    gl_Position = vec4(aPos,-0.99f,1.0f);
	TexCoord  = aTexCoord;
}
#endif


#ifdef fragment_shader
out vec4 FragColor;
in float TexCoord;

uniform sampler1D Texture;
uniform float textureCoordRatio;//纹理坐标的系数

void main()
{
    FragColor = texture(Texture,TexCoord * textureCoordRatio);
}
#endif 