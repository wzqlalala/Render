#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 pvm;

void main() {
    gl_Position = pvm * vec4(aPos, 1.0f);
}
#endif

#ifdef fragment_shader
uniform vec4 showColor;
out vec4 FragColor;

void main() {
    FragColor = showColor;
}
#endif