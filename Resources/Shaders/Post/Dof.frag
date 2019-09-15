#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	float nearPlane;
	float farPlane;

	float focusPoint;
	float nearField;
	float nearTransition;
	float farField;
	float farTransition;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerDepth;
layout(binding = 2) uniform sampler2D samplerColour;
layout(binding = 3) uniform sampler2D samplerBlured;

layout(location = 0) in vec2 inUV;

float linearDepth(float depth) {
	float z = depth * 2.0f - 1.0f;
	return (2.0f * scene.nearPlane * scene.farPlane) / (scene.farPlane + scene.nearPlane - z * (scene.farPlane - scene.nearPlane));
}

void main() {
	float depth = linearDepth(texture(samplerDepth, inUV).r);
	vec3 textureColour = texture(samplerColour, inUV).rgb;
	vec3 textureBlured = texture(samplerBlured, inUV).rgb;

	float nearEnd = scene.nearField + scene.nearTransition;
	float farStart = scene.farField - scene.farTransition;

	float nearVisibility = smoothstep(scene.nearField * scene.focusPoint, nearEnd * scene.focusPoint, depth);
	float farVisibility = 1.0f - smoothstep(farStart * scene.focusPoint, scene.farField * scene.focusPoint, depth);
	vec4 colour = vec4(mix(textureBlured, textureColour, nearVisibility), 1.0f);
	colour.rgb = mix(textureBlured, colour.rgb, farVisibility);

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
