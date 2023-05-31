#version 330 core
#ifdef vertex_shader
layout (location = 0) in vec2 aPos;
void main()
{
    gl_Position = vec4(aPos, -0.99f, 1.0f);
}
#endif

#ifdef fragment_shader
out vec4 FragColor;
uniform vec4 showColor;
void main()
{
    FragColor = showColor;
} 
#endif