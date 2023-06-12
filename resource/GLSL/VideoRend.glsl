#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in  vec2 uv;

out vec2 VertexUV;

void main()
{
	gl_Position =vec4(aPos, 1.0);
    VertexUV = uv;
}
#endif



#ifdef fragment_shader

out vec4 FragColor;

in  vec2 VertexUV;

uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main(void)
 {
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture(tex_y, VertexUV).r;
    yuv.y = texture(tex_u, VertexUV).r - 0.5;
    yuv.z = texture(tex_v, VertexUV).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    FragColor = vec4(rgb, 1) ;
}
#endif


