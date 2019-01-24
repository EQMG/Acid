#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec2 inUv;

layout(location = 0) out vec2 outUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = vec4(inPosition, 1.0f);

	outUv = inUv;
}
