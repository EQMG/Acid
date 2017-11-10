#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 fragmentTextures;
layout(location = 1) in vec3 fragmentNormal;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormal;

void main() 
{
	vec3 unitNormal = normalize(fragmentNormal);

	vec3 textureColour = texture(samplerTexture, fragmentTextures).rgb;

	outColour = vec4(textureColour, 1.0);
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}
