#version 450
#extension GL_ARB_separate_shader_objects : enable

#define PI 3.14159265f
#define EPSILON 0.001f

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

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 3) uniform sampler2D samplerColour;
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
layout(set = 0, binding = 5) uniform sampler2D samplerMaterial;
layout(set = 0, binding = 6) uniform sampler2D samplerShadows;
layout(set = 0, binding = 7) uniform sampler2D samplerBrdflut;
layout(set = 0, binding = 8) uniform samplerCube samplerEnvironment;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

float sqr(float x)
{
	return x * x;
}

vec3 decodeNormal(vec4 normal)
{
	vec2 spherical = normal.xy * 2.0f - 1.0f;
	vec2 sinCosTheta = vec2(sin(spherical.x * PI), cos(spherical.x * PI));
	vec2 sinCosPhi = vec2(sqrt(1.0f - spherical.y * spherical.y), spherical.y);
	return vec3(sinCosTheta.y * sinCosPhi.x, sinCosTheta.x * sinCosPhi.x, sinCosPhi.y);
}

vec3 decodeWorldPosition(vec2 uv, float depth)
{
	vec3 ndc = vec3(uv * 2.0f - vec2(1.0f), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0);
	return p.xyz / p.w;
}

float D_GGX(float NoH, float roughness)
{
	float a2 = sqr(sqr(roughness));
	float denom = sqr(sqr(NoH) * (a2 - 1.0f) + 1.0f) * PI;
	return a2 / denom;
}

float G_sub_GGX(float NoV, float k)
{
	float denom = NoV * (1.0f - k) + k;
	return NoV / denom;
}

float G_GGX(float NoV, float NoL, float k)
{
	// k for direct lighting = sqr(roughness + 1.0) / 8.0;
	// k for IBL = sqr(roughness) / 2.0;
	float ggx_v = G_sub_GGX(NoV, k);
	float ggx_l = G_sub_GGX(NoL, k);
	return ggx_v * ggx_l;
}

vec3 approx_F0(float metallic, vec3 albedo)
{
	return mix(vec3(0.04), albedo, metallic);
}

vec3 F_Schlick(float NoV, vec3 F0)
{
	return F0 + (vec3(1.0f) - F0) * pow(1.0f - NoV, 5.0f);
}

vec3 F_Schlick(float NoV, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - NoV, 5.0f);
}

float brdf_cook_torrance(float NoH, float NoV, float NoL, float roughness)
{
	float D = D_GGX(NoH, roughness);

	float k_direct = sqr(roughness + 1.0f) / 8.0f;
//	float k_ibl = sqr(roughness) / 2.0f;
	float G = G_GGX(NoV, NoL, k_direct);

	float denom = 4.0f * NoL * NoV + EPSILON;

	return D * G / denom;
}

float brdf_lambert()
{
	return 1.0f / PI;
}

float attenuation(float distance, float radius)
{
	if (radius > 0.0f)
	{
		float x = min(distance, radius);
		return sqr(1.0f - sqr(sqr(x / radius))) / (sqr(x) + 1.0f);
	}

	return 1.0f;
}

vec3 L0(vec3 N, vec3 L, vec3 V, float roughness, float metallic, vec3 albedo)
{
	vec3 H = normalize(L + V);
	float NoH = max(0.0f, dot(N, H));
	float NoV = max(0.0f, dot(N, V));
	float NoL = max(0.0f, dot(N, L));
	float HoV = max(0.0f, dot(H, V));

	vec3 F = F_Schlick(HoV, approx_F0(metallic, albedo));

	vec3 kS = F;
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 specular = kS * brdf_cook_torrance(NoH, NoV, NoL, roughness);
	vec3 diffuse = kD * albedo * brdf_lambert();

	return (diffuse + specular) * NoL;
}

vec3 ibl_irradiance(samplerCube probe, sampler2D brdf_lut, vec3 N, vec3 V, float roughness, float metallic, vec3 albedo)
{
	uint probe_mips = textureQueryLevels(probe);
	float NoV = max(0.0f, dot(N, V));

	vec3 kS = F_Schlick(NoV, approx_F0(metallic, albedo), roughness);
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 irradiance = textureLod(probe, N, probe_mips - 1).rgb;
	vec3 diffuse = kD * irradiance * albedo;

	vec3 reflected = reflect(-V, N);
	vec2 brdf = texture(brdf_lut, vec2(NoV, roughness)).xy;
	vec3 specular = (kS * brdf.x + brdf.y) * textureLod(probe, reflected, roughness * probe_mips).rgb;

	return diffuse + specular;
}

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
	vec4 textureDepth = texture(samplerDepth, fragmentUv);
	vec4 textureColour = texture(samplerColour, fragmentUv);
	vec4 textureNormal = texture(samplerNormal, fragmentUv);
	vec4 textureMaterial = texture(samplerMaterial, fragmentUv);

	vec3 worldPosition = decodeWorldPosition(fragmentUv, textureDepth.r);
	vec4 screenPosition = scene.view * vec4(worldPosition, 1.0f);

	float metallic = textureMaterial.r;
	float roughness = textureMaterial.g;
	bool ignoreFog = textureMaterial.b == (1.0f / 3.0f) || textureMaterial.b == (3.0f / 3.0f);
	bool ignoreLighting = textureMaterial.b == (2.0f / 3.0f) || textureMaterial.b == (3.0f / 3.0f);

	outColour = vec4(textureColour.rgb, 1.0f);

	// Lighting.
	if (!ignoreLighting && textureNormal.rgb != vec3(0.0f))
	{
        vec3 N = normalize(decodeNormal(textureNormal));
        vec3 V = normalize(scene.cameraPosition - worldPosition);

	    vec3 irradiance = vec3(0.0);

		// Point lights.
		for (int i = 0; i < scene.lightsCount; i++)
		{
            // Calculate per-light radiance.
			Light light = scene.lights[i];

			// lightDirection dot viewDirection > 0
    		vec3 L = light.position - worldPosition;
    		float LD = length(L);
    		L /= LD;
    		float att = attenuation(0.1f * LD, light.radius);

			vec3 radiance = light.colour.rgb * att;
			irradiance += radiance * L0(N, L, V, roughness, metallic, textureColour.rgb);
		}

		// Directional lights.
		/*for (int i = 0; i < scene.lightsCount; i++)
		{
            // Calculate per-light radiance.
			Light light = scene.lights[i];

			// lightDirection dot viewDirection > 0
    		vec3 L = light.position;

			vec3 radiance = light.colour.rgb;
			irradiance += radiance * L0(N, L, V, roughness, metallic, albedo);
		}*/

#ifdef USE_IBL
        irradiance += ibl_irradiance(samplerEnvironment, samplerBrdflut, N, V, roughness, metallic, textureColour.rgb);
#endif

        outColour = vec4(irradiance, 1.0f);
	}

	// Shadows.
	/*if (!ignoreLighting && scene.shadowDarkness >= 0.07f)
	{
		vec4 shadowCoords = scene.shadowSpace * vec4(worldPosition, 1.0f);
		float distanceAway = length(screenPosition.xyz);
		distanceAway = distanceAway - ((scene.shadowDistance * 2.0f) - scene.shadowTransition);
		distanceAway = distanceAway / scene.shadowTransition;
		shadowCoords.w = clamp(1.0f - distanceAway, 0.0f, 1.0f);
		outColour *= shadow(shadowCoords);
	}*/

	// Fog.
	if (!ignoreFog && textureNormal.rgb != vec3(0.0f))
	{
		float fogFactor = exp(-pow(length(screenPosition.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0f, 1.0f);
		outColour = mix(scene.fogColour, outColour, fogFactor);
	}

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
