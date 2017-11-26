#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerAlbedo;
layout(binding = 2) uniform sampler2D samplerExtras;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec4 colour = texture(samplerAlbedo, fragmentUv);
	vec3 extras = texture(samplerExtras, fragmentUv).rgb;
	outColour.rgb = colour.rgb * extras.g;
}
