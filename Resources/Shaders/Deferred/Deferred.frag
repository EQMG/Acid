#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UboScene
{
	mat4 view;
	mat4 shadowSpace;
	vec3 cameraPosition;

	int lightsCount;

	vec4 fogColour;
	float fogDensity;
	float fogGradient;
} scene;

struct Light
{
	vec4 colour;
	vec3 position;
	float radius;
};

layout(binding = 1) buffer Lights
{
	Light lights[];
} lights;

layout(binding = 2) uniform sampler2D samplerPosition;
layout(binding = 3) uniform sampler2D samplerDiffuse;
layout(binding = 4) uniform sampler2D samplerNormal;
layout(binding = 5) uniform sampler2D samplerMaterial;
layout(binding = 6) uniform sampler2D samplerBRDF;
//layout(binding = 7) uniform samplerCube samplerIrradiance;
//layout(binding = 8) uniform samplerCube samplerPrefiltered;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

const float PI = 3.1415926535897932384626433832795;

float sqr(float x)
{
	return x * x;
}

float attenuation(float distance, float radius)
{
	if (radius <= 0.0f)
	{
		return 1.0f;
	}

	float x = min(distance, radius);
	return sqr(1.0f - sqr(sqr(x / radius))) / (sqr(x) + 1.0f);
}

void main()
{
	vec3 worldPosition = texture(samplerPosition, inUV).rgb;
	vec4 screenPosition = scene.view * vec4(worldPosition, 1.0f);

	vec4 diffuse = texture(samplerDiffuse, inUV);
	vec3 normal = texture(samplerNormal, inUV).rgb;
	vec3 material = texture(samplerMaterial, inUV).rgb;

	float metallic = material.r;
	float roughness = material.g;
	bool ignoreFog = material.b == (1.0f / 3.0f) || material.b == (3.0f / 3.0f);
	bool ignoreLighting = material.b == (2.0f / 3.0f) || material.b == (3.0f / 3.0f);
	
	outColour = vec4(diffuse.rgb, 1.0f);

	if (!ignoreLighting && normal != vec3(0.0f))
	{
		vec3 V = normalize(scene.cameraPosition - worldPosition);
		vec3 R = reflect(-V, normal); 

		outColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		
		for (int i = 0; i < scene.lightsCount; i++)
		{
			Light light = lights.lights[i];
			vec3 L = normalize(light.position - worldPosition);
			float dist = length(L);
			L /= dist;
			float atten = attenuation(dist, light.radius);
			
			vec3 N = normalize(normal);
			float NdotL = max(0.0f, dot(N, L));
			vec3 diff = light.colour.rgb * diffuse.rgb * NdotL * atten;
			
			vec3 R = reflect(-L, N);
			float NdotR = max(0.0, dot(R, V));
			vec3 spec = light.colour.rgb * diffuse.a * pow(NdotR, 16.0) * atten;
			
			outColour.rgb += diff + spec;	
		}
	}

	if (!ignoreFog && normal != vec3(0.0f))
	{
		float fogFactor = exp(-pow(length(screenPosition.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0f, 1.0f);
		outColour = mix(scene.fogColour, outColour, fogFactor);
	}
}