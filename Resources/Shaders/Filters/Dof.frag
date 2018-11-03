#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float nearPlane;
	float farPlane;

	float focusPoint;
	float nearField;
	float nearTransition;
	float farField;
	float farTransition;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 4) uniform sampler2D samplerColour;
//layout(set = 0, binding = 4) uniform sampler2D samplerBlured;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0f * scene.nearPlane * scene.farPlane) / (scene.farPlane + scene.nearPlane - z * (scene.farPlane - scene.nearPlane));
}

void main() 
{
	float depth = linearDepth(texture(samplerDepth, inUv).r);
	vec3 colour = texture(samplerColour, inUv).rgb;
	vec3 blur = vec3(1.0f, 0.0f, 0.0f); // texture(samplerBlured, inUv).rgb;

	float nearEnd = scene.nearField + scene.nearTransition;
	float farStart = scene.farField - scene.farTransition;

	float nearVisibility = smoothstep(scene.nearField * scene.focusPoint, nearEnd * scene.focusPoint, depth);
	float farVisibility = 1.0f - smoothstep(farStart * scene.focusPoint, scene.farField * scene.focusPoint, depth);
	vec3 totalColour = mix(blur, colour, nearVisibility);
	totalColour = mix(blur, totalColour, farVisibility);

	outColour = vec4(totalColour, 1.0f);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
