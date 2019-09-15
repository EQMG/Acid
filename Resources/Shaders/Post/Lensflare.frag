#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	vec3 sunPosition;
	ivec2 displaySize;
	float worldHeight;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;
layout(binding = 2) uniform sampler2D samplerMaterial;

layout(location = 0) in vec2 inUV;

bool insideScreen(vec2 test) {
	return test.x >= 0.0f && test.x <= 1.0f && test.y >= 0.0f && test.y <= 1.0f;
}

void main() {
	vec2 sun2 = vec2(scene.sunPosition.x, scene.sunPosition.y);
	vec2 sunCoord = (sun2 + 1.0f) / 2.0f;

	float metallic = texture(samplerMaterial, sunCoord).r;
	bool process = scene.sunPosition.z >= 0.0f && (metallic > 0.4f || !insideScreen(sunCoord));

	vec2 uv = (inUV - 0.5f) * (scene.displaySize.x / scene.displaySize.y);

	vec2 uvd = uv * length(uv);
	vec3 flare = vec3(0.0f);

	if (process) {
		vec2 uvx = mix(uv, uvd, 1.0f);
		float f2 = max(1.0f / (1.0f + 32.0f * pow(length(uvx + 0.8f * sun2), 2.0f)), 0.0f) * 0.25f;
		float f22 = max(1.0f / (1.0f + 32.0f * pow(length(uvx + 0.85f * sun2), 2.0f)), 0.0f) * 0.23f;
		float f23 = max(1.0f / (1.0f + 32.0f * pow(length(uvx + 0.9f * sun2), 2.0f)), 0.0f) * 0.21f;

		uvx = mix(uv, uvd, -0.5f);
		float f4 = max(0.01f - pow(length(uvx + 0.4f * sun2), 2.4f), 0.0f) * 6.0f;
		float f42 = max(0.01f - pow(length(uvx + 0.45f * sun2), 2.4f), 0.0f) * 5.0f;
		float f43 = max(0.01f - pow(length(uvx + 0.5f * sun2), 2.4f), 0.0f) * 3.0f;

		uvx = mix(uv, uvd, -0.4f);
		float f5 = max(0.01f - pow(length(uvx + 0.2f * sun2), 5.5f), 0.0f) * 2.0f;
		float f52 = max(0.01f - pow(length(uvx + 0.4f * sun2), 5.5f), 0.0f) * 2.0f;
		float f53 = max(0.01f - pow(length(uvx + 0.6f * sun2), 5.5f), 0.0f) * 2.0f;

		uvx = mix(uv, uvd, -0.5f);
		float f6 = max(0.01f - pow(length(uvx - 0.3f * sun2), 1.6f), 0.0f) * 6.0f;
		float f62 = max(0.01f - pow(length(uvx - 0.325f * sun2), 1.6f), 0.0f) * 3.0f;
		float f63 = max(0.01f - pow(length(uvx - 0.35f * sun2), 1.6f), 0.0f) * 5.0f;

		flare.r += f2 + f4 + f5 + f6;
		flare.g += f22 + f42 + f52 + f62;
		flare.b += f23 + f43 + f53 + f63;
	}

	// Hides flare when below a world height.
	flare *= clamp(scene.worldHeight + 10.0f, 0.0f, 1.0f);

	// Adds a bit of darkining around the edge of the screen.
//  colour = colour * 1.3f - vec3(length(uvd) * 0.05f);

	vec4 colour = texture(samplerColour, inUV) + vec4(flare, 0.0f);

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
