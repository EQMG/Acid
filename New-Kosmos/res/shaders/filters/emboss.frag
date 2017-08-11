#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 pass_textureCoords;

layout(binding = 0) uniform sampler2D originalTexture;

layout(location = 0) out vec4 out_colour;

const vec4 LUMCOEFF = vec4(0.299, 0.587, 0.114, 0);
const float V = 0.001;

void main(void)
{
	vec4 sampler[6];
	vec2 tc_offset[6];

	tc_offset[0] = vec2(-V, -V);
	tc_offset[1] = vec2(0.0, -V);
	tc_offset[2] = vec2(-V, 0.0);
	tc_offset[3] = vec2(V, 0.0);
	tc_offset[4] = vec2(0.0, V);
	tc_offset[5] = vec2(V, V);

	for(int i = 0; i < 6; i++) 
	{
		sampler[i] = texture(originalTexture, pass_textureCoords + tc_offset[i]);
	}

	vec4 sum = vec4(0.5) + (sampler[0] + sampler[1] + sampler[2]) - (sampler[3] + sampler[4] + sampler[5]);
	float lum = dot(sum, LUMCOEFF);

	out_colour = vec4(lum, lum, lum, 1.0);
}
