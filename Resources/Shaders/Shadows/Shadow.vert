#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, location = 0) in vec3 inPosition;

layout (location = 0) out int outInstanceIndex;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	outInstanceIndex = gl_InstanceIndex;
	gl_Position = inPos;
}