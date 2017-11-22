#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 fragmentTextures;
layout(location = 1) in vec3 fragmentPosition;
layout(location = 2) in vec3 fragmentNormal;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;

vec4 encodeColour(vec3 colour, float depth)
{
	vec4 result = vec4(0.0f);
	result.r = packHalf2x16(colour.rg);
	result.g = packHalf2x16(vec2(colour.b, depth));
	result.b = 0.0f;
	result.a = 1.0f;
	return result;
}

vec2 encodeNormal(vec3 normal)
{
	vec2 result = vec2(0.0f);
	result.x = atan(normal.y, normal.x) / 3.14159f;
	result.y = normal.z;
	return result * 0.5f + 0.5f;
}

void main() 
{
	vec3 textureColour = texture(samplerTexture, fragmentTextures).rgb;
	vec3 unitNormal = normalize(fragmentNormal);

	outColour = encodeColour(textureColour, length(fragmentPosition));
	outNormal = encodeNormal(unitNormal);
}
