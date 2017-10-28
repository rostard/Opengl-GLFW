#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6)out;

in VS_OUT{
    vec3 Normal;
}gs_in[];


void DrawLine(int i){
    float magnitude = 0.004;
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
    gl_Position = gl_in[i].gl_Position + vec4(gs_in[i].Normal, 0.0) * magnitude;
    EmitVertex();
    EndPrimitive();
}

void main() {
    DrawLine(0);
    DrawLine(1);
    DrawLine(2);
}
