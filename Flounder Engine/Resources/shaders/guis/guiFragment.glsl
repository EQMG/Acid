#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D guiTexture;
uniform bool polygonMode;
uniform float alpha;
uniform vec3 colourOffset;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	out_colour = texture(guiTexture, pass_textureCoords) + vec4(colourOffset, 0.0);
	out_colour.a *= alpha;

	if (polygonMode) {
		out_colour = vec4(1.0, 0.0, 0.0, alpha);
	}

	if (out_colour.a < 0.05){
		out_colour = vec4(0.0);
		discard;
	}
}
