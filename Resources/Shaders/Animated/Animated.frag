#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
    mat4 jointTransforms[MAX_JOINTS];
	mat4 transform;
} object;

layout(location = 0) in vec2 fragmentUv;
layout(location = 1) in vec3 fragmentNormal;

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
	vec4 textureColour = vec4(0.94f, 0.94f, 0.94f, 1.0f);
	vec3 unitNormal = normalize(fragmentNormal);
	vec3 material = vec3(0.0f, 1.0f, 0.0f);

	outColour = textureColour;
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(material, 1.0f);
}
