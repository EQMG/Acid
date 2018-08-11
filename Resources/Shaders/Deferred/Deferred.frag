#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

struct Light
{
	vec4 colour;
	vec3 position;
	float radius;
};

layout(set = 0, binding = 0) uniform UboScene
{
	Light lights[MAX_LIGHTS];

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

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 2) uniform sampler2D samplerPosition;
layout(set = 0, binding = 3) uniform sampler2D samplerAlbedo;
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
layout(set = 0, binding = 5) uniform sampler2D samplerMaterial;
layout(set = 0, binding = 6) uniform sampler2D samplerShadows;
layout(set = 0, binding = 7) uniform sampler2D samplerBrdf;
layout(set = 0, binding = 8) uniform samplerCube samplerIbl;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

#include "Shaders/Lighting.glsl"

float shadow(vec4 shadowCoords)
{
	float total = 0.0f;
	vec2 sizeShadows = 1.0f / textureSize(samplerShadows, 0);
	float totalTextels = (scene.shadowPCF * 2.0f + 1.0f) * (scene.shadowPCF * 2.0f + 1.0f);

   // if (shadowCoords.x > 0.0f && shadowCoords.x < 1.0f && shadowCoords.y > 0.0f && shadowCoords.y < 1.0f && shadowCoords.z > 0.0f && shadowCoords.z < 1.0f)
   // {
		for (int x = -scene.shadowPCF; x <= scene.shadowPCF; x++)
		{
			for (int y = -scene.shadowPCF; y <= scene.shadowPCF; y++)
			{
				float shadowValue = texture(samplerShadows, shadowCoords.xy + vec2(x, y) * sizeShadows).r;

				if (shadowCoords.z < shadowValue + scene.shadowBias)
				{
					total += scene.shadowDarkness * shadowCoords.w;
				}
			}
		}

		total /= totalTextels;
   // }
   // else
   // {
   //	 total = 0.0f;
   // }

	return 1.0f - total;
}

void main()
{
	vec3 worldPosition = texture(samplerPosition, inUv).rgb;
	vec4 screenPosition = scene.view * vec4(worldPosition, 1.0f);

	vec4 albedo = texture(samplerAlbedo, inUv);
	vec3 normal = texture(samplerNormal, inUv).rgb;
	vec3 material = texture(samplerMaterial, inUv).rgb;

	float metallic = material.r;
	float roughness = material.g;
	bool ignoreFog = material.b == (1.0f / 3.0f) || material.b == (3.0f / 3.0f);
	bool ignoreLighting = material.b == (2.0f / 3.0f) || material.b == (3.0f / 3.0f);

	outAlbedo = vec4(albedo.rgb, 1.0f);

	// Lighting.
	if (!ignoreLighting && normal != vec3(0.0f))
	{
	    vec3 irradiance = vec3(0.0);
        vec3 viewDir = normalize(scene.cameraPosition - worldPosition);

		// Point lights.
		for (int i = 0; i < scene.lightsCount; i++)
		{
			Light light = scene.lights[i];

    		vec3 lightDir = light.position - worldPosition;
    		float distance = length(lightDir);
    		lightDir /= distance;

    		float att = attenuation(distance, light.radius);

			vec3 radiance = light.colour.rgb * att;
			irradiance += radiance * L0(normal, lightDir, viewDir, roughness, metallic, albedo.rgb);
		}

		// Directional lights.
		for (int i = 0; i < scene.lightsCount; i++)
		{
			Light light = scene.lights[i];

    		vec3 lightDir = light.position;

			vec3 radiance = light.colour.rgb;
			irradiance += radiance * L0(normal, lightDir, viewDir, roughness, metallic, albedo.rgb);
		}

#ifdef USE_IBL
        irradiance += ibl_irradiance(samplerIbl, samplerBrdf, normal, viewDir, roughness, metallic, albedo.rgb);
#endif

        outAlbedo = vec4(irradiance, 1.0f);
	}

	// Shadows.
	/*if (!ignoreLighting && scene.shadowDarkness >= 0.07f)
	{
		vec4 shadowCoords = scene.shadowSpace * vec4(worldPosition, 1.0f);
		float distanceAway = length(screenPosition.xyz);
		distanceAway = distanceAway - ((scene.shadowDistance * 2.0f) - scene.shadowTransition);
		distanceAway = distanceAway / scene.shadowTransition;
		shadowCoords.w = clamp(1.0f - distanceAway, 0.0f, 1.0f);
		outAlbedo *= shadow(shadowCoords);
	}*/

	// Fog.
	if (!ignoreFog && normal != vec3(0.0f))
	{
		float fogFactor = exp(-pow(length(screenPosition.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0f, 1.0f);
		outAlbedo = mix(scene.fogColour, outAlbedo, fogFactor);
	}

	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
