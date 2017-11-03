#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	float shineDamper;
	float reflectivity;
} object;

layout(binding = 2) uniform sampler2D samplerGrass;
layout(binding = 3) uniform sampler2D samplerSnow;
layout(binding = 4) uniform sampler2D samplerSand;
layout(binding = 5) uniform sampler2D samplerRock;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) in float fragmentHeight;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormals;
layout(location = 2) out vec4 outExtras;

const vec2 lightBias = vec2(0.8, 0.2);
const vec3 lightDirection = vec3(0.2, -0.3, 0.2);

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
	const vec3 tintGrass = texture(samplerGrass, fragmentTextures).rgb;

	vec3 tint = tintGrass;
	
	if (fragmentHeight <= 3.0f)
	{
		const vec3 tintSand = texture(samplerSand, fragmentTextures).rgb;
		float blendSand = clamp(fragmentHeight - 1.0f, 0.0f, 1.0f);
		tint = blend(tintSand, tint, blendSand);
	}
	/*else if (fragmentHeight >= 24.0f)
	{
		const vec3 tintSnow = texture(samplerSnow, fragmentTextures).rgb;
		float blendSnow = clamp(fragmentHeight - 24.0f, 0.0f, 1.0f);
		tint = blend(tint, tintSnow, blendSnow);
	}*/

	const vec3 tintRock = texture(samplerSand, fragmentTextures).rgb;
	float blendRock = clamp(abs(fragmentNormal.y), 0.0f, 1.0f);
	tint = blend(tintRock, tint, blendRock);

	vec3 unitNormal = normalize(fragmentNormal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;

	outColour = vec4(tint * diffuseLight, 1.0);
	outNormals = vec4(fragmentNormal + 1.0 / 2.0, outColour.a);
	outExtras = vec4(object.shineDamper, object.reflectivity, 0.0, outColour.a);
}
