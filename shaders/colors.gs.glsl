#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;

}gs_in[];

uniform float time;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

vec3 getNormal(){
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[2].gl_Position);
    return normalize(cross(b, a));
}

vec4 explode(vec4 position, vec3 normal){
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time)+1) / 2) * magnitude;
    return position + vec4(direction, 0.0);

}

void main() {
    Normal = getNormal();
    for(int i=0; i<3; i++){
        //Normal = normalize(gs_in[0].Normal);
        gl_Position = explode(gl_in[i].gl_Position, Normal);
        TexCoords = gs_in[i].TexCoords;
        FragPos = gs_in[i].FragPos;
        EmitVertex();
    }
    EndPrimitive();
}