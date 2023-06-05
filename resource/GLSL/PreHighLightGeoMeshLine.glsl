#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(aPos, 1.0f);
}
#endif

#ifdef geometry_shader
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float lineWidth;

void main() 
{    
	vec2 dir = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy) * (lineWidth/2.0)/500.0;
    vec2 normal = vec2(-dir.y, dir.x);
	
	gl_Position = gl_in[0].gl_Position + vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    EndPrimitive();

}

#endif

#ifdef fragment_shader
uniform vec4 showColor;
out vec4 FragColor;

void main()
{	
    FragColor = showColor;
} 

#endif


