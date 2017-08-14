#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColour;

layout(location = 0) out vec4 outColour;

void main() 
{
	outColour = vec4(fragColour, 1.0);
}
