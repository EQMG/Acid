#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) in vec3 fragmentColour;
layout(location = 3) in float fragmentHeight;

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
	vec3 unitNormal = normalize(fragmentNormal);

	outColour = vec4(fragmentColour, 1.0);
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}
