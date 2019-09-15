#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	vec2 direction;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	vec2 sizeColour = textureSize(samplerColour, 0);

	vec4 colour = vec4(0.0f);
#if BLUR_TYPE == 5
	vec2 off1 = 1.3333333333333333f * scene.direction;
	colour += texture(samplerColour, inUV) * 0.29411764705882354;
	colour += texture(samplerColour, inUV + (off1 / sizeColour)) * 0.35294117647058826f;
	colour += texture(samplerColour, inUV - (off1 / sizeColour)) * 0.35294117647058826f;
#elif BLUR_TYPE == 9
	vec2 off1 = 1.3846153846f * scene.direction;
	vec2 off2 = 3.2307692308f * scene.direction;
	colour += texture(samplerColour, inUV) * 0.2270270270f;
	colour += texture(samplerColour, inUV + (off1 / sizeColour)) * 0.3162162162f;
	colour += texture(samplerColour, inUV - (off1 / sizeColour)) * 0.3162162162f;
	colour += texture(samplerColour, inUV + (off2 / sizeColour)) * 0.0702702703f;
	colour += texture(samplerColour, inUV - (off2 / sizeColour)) * 0.0702702703f;
#elif BLUR_TYPE == 13
	vec2 off1 = 1.411764705882353f * scene.direction;
	vec2 off2 = 3.2941176470588234f * scene.direction;
	vec2 off3 = 5.176470588235294f * scene.direction;
	colour += texture(samplerColour, inUV) * 0.1964825501511404f;
	colour += texture(samplerColour, inUV + (off1 / sizeColour)) * 0.2969069646728344f;
	colour += texture(samplerColour, inUV - (off1 / sizeColour)) * 0.2969069646728344f;
	colour += texture(samplerColour, inUV + (off2 / sizeColour)) * 0.09447039785044732f;
	colour += texture(samplerColour, inUV - (off2 / sizeColour)) * 0.09447039785044732f;
	colour += texture(samplerColour, inUV + (off3 / sizeColour)) * 0.010381362401148057f;
	colour += texture(samplerColour, inUV - (off3 / sizeColour)) * 0.010381362401148057f;
#endif

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
