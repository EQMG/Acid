#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 diffuseColour;
} object;

layout(binding = 2) uniform sampler2D samplerTexture;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormal;

void main(void) 
{
	vec3 unitNormal = normalize(fragmentNormal);

	const vec3 tintWater = texture(samplerTexture, fragmentTextures).rgb;
	
	outColour = vec4(tintWater, 1.0); // diffuseColour
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}