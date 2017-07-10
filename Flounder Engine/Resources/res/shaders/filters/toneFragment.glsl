#version

//---------CONSTANT------------
const float exposure = 1.3;
const vec3 white = vec3(1.0, 1.0, 1.0);

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------TONE MAP------------
vec3 toneMap(vec3 color) {
	return color / (1.0 + color);
}

//---------MAIN------------
void main(void) {
	vec3 colour = texture(originalTexture, pass_textureCoords).rgb;
	out_colour = vec4(toneMap(colour * exposure) / toneMap(white), 1.0);
}
