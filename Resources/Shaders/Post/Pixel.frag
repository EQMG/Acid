#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	float pixelSize;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	vec2 sizeColour = textureSize(samplerColour, 0);

	float dx = scene.pixelSize * (1.0f / sizeColour.x);
	float dy = scene.pixelSize * (sizeColour.x / sizeColour.y) * (1.0f / sizeColour.y);
	vec2 coord = vec2(dx * floor(inUV.x / dx), dy * floor(inUV.y / dy));

	vec4 colour = vec4(texture(samplerColour, coord).rgb, 1.0f);

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
