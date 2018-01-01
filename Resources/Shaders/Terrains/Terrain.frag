#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

flat layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentUv;
layout(location = 2) in vec3 fragmentColour;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec3 outMaterial;

vec3 blend(vec3 left, vec3 right, float blend)
{
	vec3 result = vec3(0.0, 0.0, 0.0);
	result.r = ((1.0f - blend) * left.r) + (blend * right.r);
	result.g = ((1.0f - blend) * left.g) + (blend * right.g);
	result.b = ((1.0f - blend) * left.b) + (blend * right.b);
	return result;
}

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
	vec3 unitNormal = normalize(fragmentNormal);

	outColour = encodeColour(fragmentColour);
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec3(0.0f);
}
