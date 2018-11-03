#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	vec3 kernel[SSAO_KERNEL_SIZE];

	mat4 projection;
	mat4 view;

	float nearPlane;
	float farPlane;
} scene;

layout(set = 0, binding = 1, rgba8) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 3) uniform sampler2D samplerNormal;
layout(set = 0, binding = 4) uniform sampler2D samplerNoise;

layout(location = 0) in vec2 inUv;

vec3 depthToWorld(vec2 uv, float depth)
{
	vec3 ndc = vec3(uv * 2.0f - vec2(1.0f), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0f);
	return p.xyz / p.w;
}

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0f * scene.nearPlane * scene.farPlane) / (scene.farPlane + scene.nearPlane - z * (scene.farPlane - scene.nearPlane));
}

void main() 
{
	// Get G-Buffer values.
	float depth = texture(samplerDepth, inUv).r;
	vec3 worldPosition = depthToWorld(inUv, depth).xyz;

	vec3 normal = texture(samplerNormal, inUv).rgb;

	// Get a random vector using a noise lookup.
	ivec2 texDim = textureSize(samplerDepth, 0);
	ivec2 noiseDim = textureSize(samplerNoise, 0);
	vec2 noiseUv = vec2(float(texDim.x) / float(noiseDim.x), float(texDim.y) / (noiseDim.y)) * inUv;
	vec3 randomVec = texture(samplerNoise, noiseUv).rgb * 2.0f - 1.0f;

	// Create TBN matrix.
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 TBN = mat3(tangent, bitangent, normal);

	// Calculate occlusion value.
	float occlusion = 0.0f;

	for (int i = 0; i < SSAO_KERNEL_SIZE; i++)
	{
		vec3 samplePos = TBN * scene.kernel[i];
		samplePos = samplePos * SSAO_RADIUS + worldPosition;

		// project
		vec4 offset = vec4(samplePos, 1.0f);
		offset = scene.projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;

		// Sample depth.
		float sampleDepth = -linearDepth(texture(samplerDepth, offset.xy).r);

		// Range check.
		float rangeCheck = smoothstep(0.0f, 1.0f, SSAO_RADIUS / abs(worldPosition.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z ? 1.0f : 0.0f) * rangeCheck;
	}

	occlusion = 1.0f - (occlusion / float(SSAO_KERNEL_SIZE));
	vec4 colour = vec4(texture(samplerNoise, inUv).rgb, 1.0f); // occlusion, occlusion, occlusion
	
	vec2 sizeColour = textureSize(samplerNormal, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), colour);
}
