#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform UboObject
{
#ifdef ANIMATED
	mat4 jointTransforms[MAX_JOINTS];
#endif
	mat4 transform;

	vec4 baseDiffuse;
	float metallic;
	float roughness;
	float ignoreFog;
	float ignoreLighting;
} object;

#ifdef DIFFUSE_MAPPING
layout(set = 0, binding = 2) uniform sampler2D samplerDiffuse;
#endif
#ifdef MATERIAL_MAPPING
layout(set = 0, binding = 3) uniform sampler2D samplerMaterial;
#endif
#ifdef NORMAL_MAPPING
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
#endif

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNormal;
#ifdef NORMAL_MAPPING
layout(location = 3) in vec3 inTangent;
#endif

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main()
{
	vec4 diffuse = object.baseDiffuse;
	vec3 normal = inNormal;
	vec3 material = vec3(object.metallic, object.roughness, 0.0f);
	float glowing = 0.0f;

#ifdef DIFFUSE_MAPPING
	diffuse = texture(samplerDiffuse, inUv);
#endif

#ifdef MATERIAL_MAPPING
	vec4 textureMaterial = texture(samplerMaterial, inUv);
	material.x *= textureMaterial.r;
	material.y *= textureMaterial.g;

	if (textureMaterial.b > 0.5f)
	{
		glowing = 1.0f;
	}
#endif

#ifdef NORMAL_MAPPING
	vec3 N = normalize(inNormal);
	N.y = -N.y;
	vec3 T = normalize(inTangent);
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	normal = TBN * normalize(texture(samplerNormal, inUV).rgb * 2.0f - vec3(1.0f));
#endif

	material.z = (1.0f / 3.0f) * (object.ignoreFog + (2.0f * min(object.ignoreLighting + glowing, 1.0f)));

	outPosition = vec4(inWorldPos, 1.0f);
	outDiffuse = diffuse;
	outNormal = vec4(normal, 1.0f);
	outMaterial = vec4(material, 1.0f);
}
