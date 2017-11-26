#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 fragmentTextures;
layout(location = 1) in vec3 fragmentNormal;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec3 outExtras;

vec4 encodeColour(vec3 colour)
{
	vec4 result = vec4(0.0f);
	result.rgb = colour;
	result.a = 1.0f;
	return result;
}

vec2 encodeNormal(vec3 normal)
{
	vec2 result = vec2(0.0f);
	result.x = (atan(normal.y, normal.x) / 3.14159f) * 0.5f + 0.5f;
	result.y = normal.z * 0.5f + 0.5f;
	return result;
}

void main() 
{
	vec3 textureColour = texture(samplerTexture, fragmentTextures).rgb;
	vec3 unitNormal = normalize(fragmentNormal);

	outColour = encodeColour(textureColour);
	outNormal = encodeNormal(unitNormal);
	outExtras = vec3(0.0f);
}
