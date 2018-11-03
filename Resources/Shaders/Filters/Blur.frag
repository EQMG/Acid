#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	vec2 direction;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main()
{
	vec2 sizeColour = textureSize(samplerColour, 0);

	outColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
#if BLUR_TYPE == 5
	vec2 off1 = 1.3333333333333333f * scene.direction;
	outColour += texture(samplerColour, inUv) * 0.29411764705882354;
	outColour += texture(samplerColour, inUv + (off1 / sizeColour)) * 0.35294117647058826f;
	outColour += texture(samplerColour, inUv - (off1 / sizeColour)) * 0.35294117647058826f;
#elif BLUR_TYPE == 9
	vec2 off1 = 1.3846153846f * scene.direction;
	vec2 off2 = 3.2307692308f * scene.direction;
	outColour += texture(samplerColour, inUv) * 0.2270270270f;
	outColour += texture(samplerColour, inUv + (off1 / sizeColour)) * 0.3162162162f;
	outColour += texture(samplerColour, inUv - (off1 / sizeColour)) * 0.3162162162f;
	outColour += texture(samplerColour, inUv + (off2 / sizeColour)) * 0.0702702703f;
	outColour += texture(samplerColour, inUv - (off2 / sizeColour)) * 0.0702702703f;
#elif BLUR_TYPE == 13
	vec2 off1 = 1.411764705882353f * scene.direction;
	vec2 off2 = 3.2941176470588234f * scene.direction;
	vec2 off3 = 5.176470588235294f * scene.direction;
	outColour += texture(samplerColour, inUv) * 0.1964825501511404f;
	outColour += texture(samplerColour, inUv + (off1 / sizeColour)) * 0.2969069646728344f;
	outColour += texture(samplerColour, inUv - (off1 / sizeColour)) * 0.2969069646728344f;
	outColour += texture(samplerColour, inUv + (off2 / sizeColour)) * 0.09447039785044732f;
	outColour += texture(samplerColour, inUv - (off2 / sizeColour)) * 0.09447039785044732f;
	outColour += texture(samplerColour, inUv + (off3 / sizeColour)) * 0.010381362401148057f;
	outColour += texture(samplerColour, inUv - (off3 / sizeColour)) * 0.010381362401148057f;
#endif

	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
