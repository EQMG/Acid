#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec3 fragmentColour;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

vec2 encodeNormal(vec3 normal)
{
	vec2 result = vec2(0.0f);
	result.x = (atan(normal.y, normal.x) / 3.14159f) * 0.5f + 0.5f;
	result.y = normal.z * 0.5f + 0.5f;
	return result;
}

void main() 
{
	outColour = vec4(fragmentColour, 1.0f);
	outNormal = encodeNormal(fragmentNormal);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
