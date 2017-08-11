#version 450
#extension GL_ARB_separate_shader_objects : enable

//---------CONSTANT------------
const float DI = 1.0 / 64.0;

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float moveIt;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void)
{
	vec2 t0 = pass_textureCoords;
	t0.x += cos(2.0 * 3.14 * pass_textureCoords.y * 4.0 + moveIt) * DI;
	t0.y += sin(2.0 * 3.14 * pass_textureCoords.x * 4.0 + moveIt) * DI;
	out_colour = texture(originalTexture, t0);
}
