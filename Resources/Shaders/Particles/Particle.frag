#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outMaterial;

void main() 
{
	vec3 colour = texture(samplerColour, inUv).rgb;

	outDiffuse = vec4(colour, 1.0f);
	outNormal = vec4(0.0f);
	outMaterial = vec4(0.0f);
}
