#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out float outShadow;

void main(void) 
{
	outShadow = 1.0f;
}