#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	vec3 colour = texture(originalTexture, pass_textureCoords).rgb;
	float grey = dot(colour, vec3(0.299, 0.587, 0.114));
	out_colour = vec4(grey * vec3(1.2, 1.0, 0.8), 1.0);
}
