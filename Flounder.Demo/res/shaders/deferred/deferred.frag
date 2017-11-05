#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerColour;
layout(binding = 1) uniform sampler2D samplerNormal;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec4 colour = texture(samplerColour, fragmentTextures);
	vec4 normal = texture(samplerNormal, fragmentTextures);
	float grey = dot(colour.rgb, vec3(0.299, 0.587, 0.114));
	
	outColour = vec4(grey, grey, grey, 1.0);
	// outColour = vec4(colour.rgb, 1.0);
	// outColour = vec4(1.0, 0.0, 0.0, 1.0);
}
