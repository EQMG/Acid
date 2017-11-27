#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS 64
#define PI 3.141592653589793f
#define EPSILON 0.001f

struct Light 
{
	vec4 colour;
	vec3 position;
	float radius;
};

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
	int shadowMapSize;
	int shadowPCF;

	int lightsCount;
} scene;

layout(set = 0, binding = 1) uniform UboLights 
{
	Light lights[MAX_LIGHTS];
} lights;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 3) uniform sampler2D samplerColour;
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;
layout(set = 0, binding = 5) uniform sampler2D samplerMaterial;
layout(set = 0, binding = 6) uniform sampler2D samplerShadows;

layout(rgba16f, set = 0, binding = 3) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

float sqr(float x) 
{
	return x * x;
}

vec3 decodeNormal(vec4 normal)
{
	vec2 spherical = normal.xy * 2.0f - 1.0f;
	vec2 sinCosTheta = vec2(sin(spherical.x * 3.14159f), cos(spherical.x * 3.14159f));
	vec2 sinCosPhi = vec2(sqrt(1.0f - spherical.y * spherical.y), spherical.y);
	return vec3(sinCosTheta.y * sinCosPhi.x, sinCosTheta.x * sinCosPhi.x, sinCosPhi.y);
}

vec3 decodeWorldPosition(vec2 uv, float depth) 
{
	vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0);
	return p.xyz / p.w;
}

float attenuation(float radius, float distance)
{
	if (radius != -1.0f)
	{
		float x = min(distance, radius);
		return sqr(1.0 - sqr(sqr(x / radius))) / (sqr(x) + 1.0);
	}

	return 1.0f;
}

void main() 
{
	vec4 textureDepth = texture(samplerDepth, fragmentUv);
	vec4 textureColour = texture(samplerColour, fragmentUv);
	vec4 textureNormal = texture(samplerNormal, fragmentUv);
	vec4 textureMaterial = texture(samplerMaterial, fragmentUv);

	vec3 colour = textureColour.rgb;
	vec3 normal = decodeNormal(textureNormal);
	float depth = textureDepth.r;
	float metallic = textureMaterial.r;
	float roughness = textureMaterial.g;
	vec3 worldPosition = decodeWorldPosition(fragmentUv, depth);
	vec3 viewDirection = normalize(scene.cameraPosition - worldPosition);
	vec4 positionRelativeToCam = scene.view * vec4(worldPosition, 1.0f);
	
	outColour = vec4(colour, 1.0f);

	// Lighting.
	if (textureNormal.rgb != vec3(0.0f))
	{
		vec3 irradiance = vec3(0.0f);

		for (int i = 0; i < scene.lightsCount; i++) 
		{
			Light light = lights.lights[i];
			
			vec3 lightDirection = light.position - worldPosition;
			float distance = length(lightDirection);
			lightDirection /= distance;
			
			float att = attenuation(light.radius, distance);
			
			vec3 LvD = normalize(lightDirection + viewDirection);
			float NoH = max(0.0f, dot(normal, LvD));
			float NoV = max(0.0f, dot(normal, viewDirection));
			float NoL = max(0.0f, dot(normal, lightDirection));
			float HoV = max(0.0f, dot(LvD, viewDirection));

			vec3 F0 = mix(vec3(0.04f), colour, metallic);
			vec3 F = F0 + (vec3(1.0f) - F0) * pow(1.0f - HoV, 5.0f);

			vec3 kS = F;
			vec3 kD = (vec3(1.0) - kS) * (1.0f - metallic);

			float D0 = sqr(sqr(roughness));
			float D = D0 / (sqr(sqr(NoH) * (D0 - 1.0f) + 1.0f) * PI);

			float k = sqr(roughness + 1.0f) / 8.0f;
			float ggx_v = NoV / (NoV * (1.0f - k) + k);
			float ggx_l = NoL / (NoV * (1.0f - k) + k);
			float G = ggx_v * ggx_l;

			vec3 specular = kS * D * G / (4.0f * NoL * NoV + EPSILON);
			vec3 diffuse = kD * colour * (1.0f / PI);

			vec3 L0 = (diffuse + specular) * NoL;
			vec3 radiance = light.colour.rgb * att;
			irradiance += radiance * L0;
		}

		outColour = vec4(irradiance, 1.0f);
	}

	// Fog.
	if (textureNormal.rgb != vec3(0.0f))
	{
		float fogFactor = exp(-pow(length(positionRelativeToCam.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		outColour = mix(scene.fogColour, outColour, fogFactor);
	}

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
