#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(binding = 2) uniform sampler2D samplerGrass;
layout(binding = 3) uniform sampler2D samplerSand;
layout(binding = 4) uniform sampler2D samplerRock;

layout(location = 0) flat in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) flat in float fragmentHeight;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormal;

vec3 blend(vec3 left, vec3 right, float blend)
{
	vec3 result = vec3(0.0, 0.0, 0.0);
	result.r = ((1.0f - blend) * left.r) + (blend * right.r);
	result.g = ((1.0f - blend) * left.g) + (blend * right.g);
	result.b = ((1.0f - blend) * left.b) + (blend * right.b);
	return result;
}

void main(void) 
{
	const vec3 tintGrass = vec3(0.10, 0.38, 0.05); // texture(samplerGrass, fragmentTextures).rgb;
	const vec3 tintSand = vec3(0.60, 0.17, 0.09); // texture(samplerSand, fragmentTextures).rgb;
	const vec3 tintRock = vec3(0.35, 0.26, 0.2); // texture(samplerRock, fragmentTextures).rgb;
	
	vec3 tint = tintSand;
	
	/*if (fragmentHeight <= 3.0f)
	{
		float blendSand = clamp(fragmentHeight - 1.0f, 0.0f, 1.0f);
		tint = blend(tintGrass, tint, blendSand);
	}*/

	float blendRock = clamp(abs(fragmentNormal.y), 0.0f, 1.0f);
	tint = blend(tintRock, tint, blendRock);

	vec3 unitNormal = normalize(fragmentNormal);

	outColour = vec4(tint, 1.0);
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}
