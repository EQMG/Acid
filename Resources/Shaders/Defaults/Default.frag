#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform UboObject
{
#if ANIMATED
	mat4 jointTransforms[MAX_JOINTS];
#endif
	mat4 transform;

	vec4 baseDiffuse;
	float metallic;
	float roughness;
	float ignoreFog;
	float ignoreLighting;
} object;

#if DIFFUSE_MAPPING
layout(binding = 2) uniform sampler2D samplerDiffuse;
#endif
#if MATERIAL_MAPPING
layout(binding = 3) uniform sampler2D samplerMaterial;
#endif
#if NORMAL_MAPPING
layout(binding = 4) uniform sampler2D samplerNormal;
#endif

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNormal;
#if NORMAL_MAPPING
layout(location = 3) in vec3 inTangent;
#endif

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main()
{
	vec4 diffuse = object.baseDiffuse;
	vec3 normal = normalize(inNormal);
	vec3 material = vec3(object.metallic, object.roughness, 0.0f);
	float glowing = 0.0f;

#if DIFFUSE_MAPPING
	diffuse = texture(samplerDiffuse, inUv);
#endif

#if MATERIAL_MAPPING
	vec4 textureMaterial = texture(samplerMaterial, inUv);
	material.x *= textureMaterial.r;
	material.y *= textureMaterial.g;

	if (textureMaterial.b > 0.5f)
	{
		glowing = 1.0f;
	}
#endif

#if NORMAL_MAPPING // TODO: Fix normal mapping.
	/*vec3 N = normalize(inNormal);
	N.y = -N.y;
	vec3 T = normalize(inTangent);
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	normal = TBN * normalize(texture(samplerNormal, inUv).rgb * 2.0f - vec3(1.0f));*/
#endif

	material.z = (1.0f / 3.0f) * (object.ignoreFog + (2.0f * min(object.ignoreLighting + glowing, 1.0f)));

	outPosition = inPosition;
	outDiffuse = diffuse;
	outNormal = vec4(normalize(normal), 1.0f);
	outMaterial = vec4(material, 1.0f);
}
