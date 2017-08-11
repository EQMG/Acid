#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 pass_textureCoords;

layout(binding = 0) uniform sampler2D originalTexture;

layout(location = 0) out vec4 out_colour;

void main(void) 
{
	vec3 colour = texture(originalTexture, pass_textureCoords).rgb;
	float grey = dot(colour, vec3(0.299, 0.587, 0.114));
	out_colour = vec4(grey, grey, grey, 1.0);
}
