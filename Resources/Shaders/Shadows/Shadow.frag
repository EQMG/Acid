#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outShadow;

void main() 
{
	outShadow = vec4(1.0f);
}