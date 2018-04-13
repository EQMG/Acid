#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, location = 0) in vec3 vertexPosition;
layout(set = 0, location = 1) in vec2 vertexUv;
//layout(set = 0, location = 2) in mat4 vertexMvp;

layout(location = 0) out vec2 fragmentUv;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
   // gl_Position = vertexMvp * vec4(vertexPosition, 1.0f);

    fragmentUv = vertexUv;
}
