#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	mat4 shadowSpace;

	vec4 fogColour;
	vec3 cameraPosition;
	float fogDensity;
	float fogGradient;

	float shadowDistance;
	float shadowTransition;
	float shadowBias;
	float shadowDarkness;
	int shadowPCF;

	int lightsCount;
} scene;

struct Light
{
	vec4 colour;
	vec3 position;
	float radius;
};

layout(set = 0, binding = 1) buffer Lights
{
	Light lights[];
} lights;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 3) uniform sampler2D samplerDiffuse;
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
layout(set = 0, binding = 5) uniform sampler2D samplerMaterial;
layout(set = 0, binding = 6) uniform sampler2D samplerShadows;
#if USE_IBL
layout(set = 0, binding = 7) uniform sampler2D samplerBrdf;
layout(set = 0, binding = 8) uniform samplerCube samplerIbl;
#endif

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

#include "Shaders/Lighting.glsl"

vec3 depthToWorld(vec2 uv, float depth)
{
	vec3 ndc = vec3(uv * 2.0f - vec2(1.0f), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0f);
	return p.xyz / p.w;
}

/*float shadow(vec4 shadowCoords)
{
	vec2 sizeShadows = 1.0f / textureSize(samplerShadows, 0);
	float totalTextels = (scene.shadowPCF * 2.0f + 1.0f) * (scene.shadowPCF * 2.0f + 1.0f);

	float total = 0.0f;
	
	if (shadowCoords.x > 0.0f && shadowCoords.x < 1.0f && shadowCoords.y > 0.0f && shadowCoords.y < 1.0f && shadowCoords.z > 0.0f && shadowCoords.z < 1.0f)
	{
		float shadowValue = texture(samplerShadows, shadowCoords.xy * sizeShadows).r;

		if (shadowCoords.z < shadowValue + scene.shadowBias)
		{
			total += scene.shadowDarkness * shadowCoords.w;
		}
	}
	else
	{
		 total = 0.0f;
	}

	return 1.0f - total;
}*/

void main()
{
	float depth = texture(samplerDepth, inUv).r;
	vec3 worldPosition = depthToWorld(inUv, depth).xyz;
	vec4 screenPosition = scene.view * vec4(worldPosition, 1.0f);

	vec4 diffuse = texture(samplerDiffuse, inUv);
	vec3 normal = texture(samplerNormal, inUv).rgb;
	vec3 material = texture(samplerMaterial, inUv).rgb;

	float metallic = material.r;
	float roughness = material.g;
	bool ignoreFog = material.b == (1.0f / 3.0f) || material.b == (3.0f / 3.0f);
	bool ignoreLighting = material.b == (2.0f / 3.0f) || material.b == (3.0f / 3.0f);

	outColour = vec4(diffuse.rgb, 1.0f);

	if (!ignoreLighting && normal != vec3(0.0f))
	{
		vec3 irradiance = 0.1f * diffuse.rgb; // vec3(0.0f)
		vec3 viewDir = normalize(scene.cameraPosition - worldPosition);

		for (int i = 0; i < scene.lightsCount; i++)
		{
			Light light = lights.lights[i];

			vec3 lightDir = light.position - worldPosition;
			float dist = length(lightDir);
			lightDir /= dist;

			float atten = attenuation(dist, light.radius);
			vec3 radiance = light.colour.rgb * atten;

			irradiance += radiance * L0(normal, lightDir, viewDir, roughness, metallic, diffuse.rgb);
		}

#if USE_IBL
		irradiance += ibl_irradiance(samplerIbl, samplerBrdf, normal, viewDir, roughness, metallic, diffuse.rgb);
#endif

		outColour = vec4(irradiance, 1.0f);

		/*if (scene.shadowDarkness >= 0.07f)
		{
			vec4 shadowCoords = scene.shadowSpace * vec4(worldPosition, 1.0f);
			float distanceAway = length(screenPosition.xyz);
			distanceAway = distanceAway - ((scene.shadowDistance * 2.0f) - scene.shadowTransition);
			distanceAway = distanceAway / scene.shadowTransition;
			shadowCoords.w = clamp(1.0f - distanceAway, 0.0f, 1.0f);
			outColour *= shadow(shadowCoords);
		}*/
	}

	if (!ignoreFog && normal != vec3(0.0f))
	{
		float fogFactor = exp(-pow(length(screenPosition.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0f, 1.0f);
		outColour = mix(scene.fogColour, outColour, fogFactor);
	}

//	if (inUv.x < 0.4f && inUv.y < 0.4f)
//	{
//		outColour = texture(samplerShadows, inUv / 0.4f);
//	}
}
