#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

const vec4 lumcoeff = vec4(0.299f, 0.587f, 0.114f, 0.0f);
const float epsilon = 0.001f;

void main() {
	vec4 tc_sampler[6];
	vec2 tc_offset[6];

	tc_offset[0] = vec2(-epsilon, -epsilon);
	tc_offset[1] = vec2(0.0f, -epsilon);
	tc_offset[2] = vec2(-epsilon, 0.0f);
	tc_offset[3] = vec2(epsilon, 0.0f);
	tc_offset[4] = vec2(0.0f, epsilon);
	tc_offset[5] = vec2(epsilon, epsilon);

	for (int i = 0; i < 6; i++) {
		tc_sampler[i] = texture(samplerColour, inUV + tc_offset[i]);
	}

	vec4 sum = vec4(0.5f) + (tc_sampler[0] + tc_sampler[1] + tc_sampler[2]) - (tc_sampler[3] + tc_sampler[4] + tc_sampler[5]);
	float lum = dot(sum, lumcoeff);

	vec4 colour = vec4(lum, lum, lum, 1.0f);

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
