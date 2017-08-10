#version 150 core

//---------CONSTANT------------
const float EXPOSURE = 1.3;
const vec3 WHITE = vec3(1.0, 1.0, 1.0);

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------TONE MAP------------
vec3 toneMap(vec3 colour) 
{
	return colour / (1.0 + colour);
}

//---------MAIN------------
void main(void) 
{
	vec3 colour = texture(originalTexture, pass_textureCoords).rgb;
	out_colour = vec4(toneMap(colour * EXPOSURE) / toneMap(WHITE), 1.0);
}
