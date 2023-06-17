#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 pvm;
uniform int PointSize;
// out vec3 color;
void main()
{
	gl_Position = pvm * vec4(aPos, 1.0);
	// color = aColor;
	gl_PointSize = PointSize;
}
#endif



#ifdef fragment_shader

// in vec4 color;
out vec4 FragColor;
uniform sampler2D sprite_texture;

void main()
{	
	vec4 color = texture(sprite_texture, gl_PointCoord);
	if(all(greaterThan(color.rgb, vec3(0.9))))
    {
        discard;
    }
    FragColor = color;
} 

#endif


