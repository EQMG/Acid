#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------CONSTANT------------
const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);
const float v = 0.001;

//---------MAIN------------
void main(void) {
	vec4 sampler[6];
	vec2 tc_offset[6];

	tc_offset[0] = vec2(-v, -v);
	tc_offset[1] = vec2(0.0, -v);
	tc_offset[2] = vec2(-v, 0.0);
	tc_offset[3] = vec2(v, 0.0);
	tc_offset[4] = vec2(0.0, v);
	tc_offset[5] = vec2(v, v);

	for(int i = 0; i < 6; i++) {
		sampler[i] = texture(originalTexture, pass_textureCoords + tc_offset[i]);
	}

	vec4 sum = vec4(0.5) + (sampler[0] + sampler[1] + sampler[2]) - (sampler[3] + sampler[4] + sampler[5]);
	float lum = dot(sum, lumcoeff);

	out_colour = vec4(lum, lum, lum, 1.0);
}
