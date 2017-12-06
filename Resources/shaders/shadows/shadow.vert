#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboObject 
{
	mat4 mvp;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = object.mvp * vec4(vertexPosition, 1.0f);
}