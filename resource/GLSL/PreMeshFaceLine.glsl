#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aIsTriangle;

uniform mat4 pvm;

out VS_OUT {
    int isTriangle;
    vec3 pos;
} vs_out;

void main() {
    vs_out.pos = aPos;
    gl_Position = pvm * vec4(vs_out.pos, 1.0f);

    vs_out.isTriangle = int(aIsTriangle);
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    int isTriangle;
    vec3 pos;
} gs_in[];

uniform vec4 showColor;
uniform float rightToLeft;

out vec4 fColor;

void main() {
    float ratio = rightToLeft / distance(gs_in[0].pos, gs_in[1].pos);
    fColor = (ratio < 800) ? vec4(showColor.xyz, 1 - sqrt(ratio / 800)) : vec4(showColor.xyz, 0.0);

    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    if (gs_in[0].isTriangle == 1) {
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    } 
    EndPrimitive();
}
#endif

#ifdef fragment_shader
in vec4 fColor;

out vec4 FragColor;

void main() {
    FragColor = fColor;
}
#endif