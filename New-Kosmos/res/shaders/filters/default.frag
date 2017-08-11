#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 pass_textureCoords;

layout(binding = 0) uniform sampler2D originalTexture;

layout(location = 0) out vec4 out_colour;

void main(void) 
{
	out_colour = texture(originalTexture, pass_textureCoords);
}
