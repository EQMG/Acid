#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_GOOGLE_include_directive : require

layout(binding = 0) uniform UniformScene {
	mat4 view;
	mat4 shadowSpace;
	vec3 cameraPosition;

	int lightsCount;

	vec4 fogColour;
	float fogDensity;
	float fogGradient;
} scene;

struct Light {
	vec4 colour;
	vec3 position;
	float radius;
};

layout(binding = 1) buffer BufferLights {
	Light lights[];
} bufferLights;

//layout(binding = 2) uniform sampler2D samplerShadows;
layout(binding = 3) uniform sampler2D samplerPosition;
layout(binding = 4) uniform sampler2D samplerDiffuse;
layout(binding = 5) uniform sampler2D samplerNormal;
layout(binding = 6) uniform sampler2D samplerMaterial;
layout(binding = 7) uniform sampler2D samplerBRDF;
layout(binding = 8) uniform samplerCube samplerIrradiance;
layout(binding = 9) uniform samplerCube samplerPrefiltered;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

//#include <Shaders/Noise.glsl>
#include "Lighting.glsl"

void main() {
	vec3 worldPosition = texture(samplerPosition, inUV).rgb;
	vec4 screenPosition = scene.view * vec4(worldPosition, 1.0f);

	vec4 diffuse = texture(samplerDiffuse, inUV);
	vec3 normal = texture(samplerNormal, inUV).rgb;
	vec3 material = texture(samplerMaterial, inUV).rgb;

	float metallic = material.r;
	float roughness = material.g;
	bool ignoreFog = material.b == (1.0f / 3.0f) || material.b == (3.0f / 3.0f);
	bool ignoreLighting = material.b == (2.0f / 3.0f) || material.b == (3.0f / 3.0f);
	
	vec3 N = normalize(normal);
	vec3 V = normalize(scene.cameraPosition - worldPosition);
	vec3 R = reflect(-V, N); 

	if (!ignoreLighting && normal != vec3(0.0f)) {
		vec3 F0 = vec3(0.04f); 
		F0 = mix(F0, diffuse.rgb, metallic);
		vec3 Lo = vec3(0.0f);

		for (int i = 0; i < scene.lightsCount; i++) {
			Light light = bufferLights.lights[i];
			vec3 L = light.position - worldPosition;
			float Dl = length(L);
			L /= Dl;
			Lo += attenuation(Dl, light.radius) * light.colour.rgb * specularContribution(diffuse.rgb, L, V, N, F0, metallic, roughness);
		}
	
		vec2 brdf = texture(samplerBRDF, vec2(max(dot(N, V), 0.0f), roughness)).rg;
		vec3 reflection = prefilteredReflection(R, roughness, samplerPrefiltered).rgb;	
		vec3 irradiance = texture(samplerIrradiance, N).rgb;
		
		// Diffuse based on irradiance
		vec3 albedo = irradiance * diffuse.rgb;	

		vec3 F = F_SchlickR(max(dot(N, V), 0.0f), F0, roughness);

		// Specular reflectance
		vec3 specular = reflection * (F * brdf.r + brdf.g);

		// Ambient part
		vec3 kD = 1.0f - F;
		kD *= 1.0f - metallic;	  
		vec3 ambient = (kD * albedo + specular);
	
		outColour = vec4(ambient + Lo, 1.0f);

		// Shadow mapping
		//vec4 shadowCoords = scene.shadowSpace * vec4(worldPosition, 1.0f);
		//outColour *= shadowFactor(shadowCoords);
	} else {
		outColour = vec4(diffuse.rgb, 1.0f);
	}

	if (!ignoreFog && normal != vec3(0.0f)) {
		float fogFactor = exp(-pow(length(screenPosition.xyz) * scene.fogDensity, scene.fogGradient));
		fogFactor = clamp(fogFactor, 0.0f, 1.0f);
		outColour = mix(scene.fogColour, outColour, fogFactor);
	}
}