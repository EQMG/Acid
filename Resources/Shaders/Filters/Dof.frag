#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene
{
	float nearPlane;
	float farPlane;

	float focusPoint;
	float nearField;
	float nearTransition;
	float farField;
	float farTransition;
} scene;

layout(set = 0, binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerDepth;
layout(set = 0, binding = 2) uniform sampler2D samplerColour;
layout(set = 0, binding = 3) uniform sampler2D samplerBlured;

layout(location = 0) in vec2 inUv;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0f * scene.nearPlane * scene.farPlane) / (scene.farPlane + scene.nearPlane - z * (scene.farPlane - scene.nearPlane));
}

void main() 
{
	float depth = linearDepth(texture(samplerDepth, inUv).r);
	vec3 textureColour = texture(samplerColour, inUv).rgb;
	vec3 textureBlured = texture(samplerBlured, inUv).rgb;

	float nearEnd = scene.nearField + scene.nearTransition;
	float farStart = scene.farField - scene.farTransition;

	/*float nearVisibility = smoothstep(scene.nearField * scene.focusPoint, nearEnd * scene.focusPoint, depth);
	float farVisibility = 1.0f - smoothstep(farStart * scene.focusPoint, scene.farField * scene.focusPoint, depth);
	vec4 colour = vec4(mix(textureBlured, textureColour, nearVisibility), 1.0f);
	colour.rgb = mix(textureBlured, colour.rgb, farVisibility);*/
	vec4 colour = vec4(textureBlured, 1.0f);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), colour);
}
