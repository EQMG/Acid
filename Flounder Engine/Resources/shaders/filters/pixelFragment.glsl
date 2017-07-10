#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float pixelSize;
uniform vec2 displaySize;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	vec2 coord;

	if (pass_textureCoords.x < 1.0) {
		float dx = pixelSize * (1.0 / displaySize.x);
		float dy = pixelSize * (1.0 / displaySize.y);
		coord.x = dx * floor(pass_textureCoords.x / dx);
		coord.y = dy * floor(pass_textureCoords.y / dy);
	} else if (pass_textureCoords.x > 1.0) {
		coord = pass_textureCoords;
	}

	vec3 colour = texture(originalTexture, coord).rgb;
	out_colour = vec4(colour, 1.0);
}
