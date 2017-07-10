#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float factor;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	vec3 colour = texture(originalTexture, pass_textureCoords).rgb;
	out_colour = vec4(colour * factor, 1.0);
}
