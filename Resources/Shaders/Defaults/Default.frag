#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_JOINTS 50

layout(set = 0, binding = 1) uniform UboObject
{
#ifdef ANIMATED
	mat4 jointTransforms[MAX_JOINTS];
#endif
	mat4 transform;

	vec4 baseColor;
	float metallic;
	float roughness;
	float ignoreFog;
	float ignoreLighting;

//#if SWAY_MAPPING
//	vec2 swayOffset;
//	float swayHeight;
//#endif
} object;

#ifdef COLOUR_MAPPING
layout(set = 0, binding = 2) uniform sampler2D samplerDiffuse;
#endif
#ifdef MATERIAL_MAPPING
layout(set = 0, binding = 3) uniform sampler2D samplerMaterial;
#endif
#ifdef NORMAL_MAPPING
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
#endif

layout(location = 0) in vec2 fragmentUv;
layout(location = 1) in vec3 fragmentNormal;
#ifdef NORMAL_MAPPING
layout(location = 2) in vec3 fragmentTangentT;
layout(location = 3) in vec3 fragmentTangentN;
layout(location = 4) in vec3 fragmentTangentB;
#endif
#ifdef TANGENT_AS_COLOUR
layout(location = 5) in vec3 fragmentColour;
#endif

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
	vec4 textureColour = object.baseColor;
	vec3 unitNormal = normalize(fragmentNormal);
	vec3 material = vec3(object.metallic, object.roughness, 0.0f);
	float glowing = 0.0f;

#ifdef TANGENT_AS_COLOUR
	textureColour.rgb = fragmentColour;
#endif
#ifdef COLOUR_MAPPING
	textureColour = texture(samplerDiffuse, fragmentUv);
#endif

#ifdef MATERIAL_MAPPING
	vec4 textureMaterial = texture(samplerMaterial, fragmentUv);
	material.x *= textureMaterial.r;
	material.y *= textureMaterial.g;

	if (textureMaterial.b > 0.5f)
	{
		glowing = 1.0f;
	}
#endif

#ifdef NORMAL_MAPPING
	vec4 textureNormal = texture(samplerNormal, fragmentUv);
	unitNormal = textureNormal.rgb;
	unitNormal = normalize(textureNormal.rgb * 2.0f - vec3(1.0f));
	unitNormal = normalize(mat3(fragmentTangentT, fragmentTangentN, fragmentTangentB) * unitNormal);
#endif

	material.z = (1.0f / 3.0f) * (object.ignoreFog + (2.0f * min(object.ignoreLighting + glowing, 1.0f)));

	outColour = textureColour;
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(material, 1.0f);
}
