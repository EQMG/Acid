#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	float strength;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	float x = (inUV.x + 4.0f) * (inUV.y + 4.0f) * 10.0f;
	vec4 grain = vec4(mod((mod(x, 13.0f) + 1.0f) * (mod(x, 123.0f) + 1.0f), 0.01f) - 0.005f) * scene.strength;

	vec4 colour = texture(samplerColour, inUV) + grain;
	
	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
