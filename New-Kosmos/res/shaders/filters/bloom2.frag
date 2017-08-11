#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 pass_textureCoords;

layout(binding = 0) uniform sampler2D originalTexture;
layout(binding = 1) uniform sampler2D bloomTexture;

layout(location = 0) out vec4 out_colour;

void main(void) 
{
	vec4 colour = texture(originalTexture, pass_textureCoords);
	vec4 bloomColour = texture(bloomTexture, pass_textureCoords);
	out_colour = vec4(colour.rgb + bloomColour.rgb, 1.0);
}
