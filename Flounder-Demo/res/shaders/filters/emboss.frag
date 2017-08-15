#version 450

#extension GL_ARB_separate_shader_objects : enable

const vec4 LUMCOEFF = vec4(0.299, 0.587, 0.114, 0);
const float V = 0.001;

layout(binding = 0) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void)
{
	vec4 tc_sampler[6];
	vec2 tc_offset[6];

	tc_offset[0] = vec2(-V, -V);
	tc_offset[1] = vec2(0.0, -V);
	tc_offset[2] = vec2(-V, 0.0);
	tc_offset[3] = vec2(V, 0.0);
	tc_offset[4] = vec2(0.0, V);
	tc_offset[5] = vec2(V, V);

	for(int i = 0; i < 6; i++) 
	{
		tc_sampler[i] = texture(samplerTexture, textureCoords + tc_offset[i]);
	}

	vec4 sum = vec4(0.5) + (tc_sampler[0] + tc_sampler[1] + tc_sampler[2]) - (tc_sampler[3] + tc_sampler[4] + tc_sampler[5]);
	float lum = dot(sum, LUMCOEFF);

	outColour = vec4(lum, lum, lum, 1.0);
}
