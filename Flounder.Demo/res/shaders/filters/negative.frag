#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec3 colour = texture(samplerColour, fragmentUv).rgb;
	outColour = vec4((1.0 - colour), 1.0);
}
