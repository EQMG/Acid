#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 pass_textureCoords;

layout(binding = 0) uniform sampler2D originalAlbedo;
layout(binding = 2) uniform sampler2D originalExtras;

layout(location = 0) out vec4 out_colour;

void main(void) 
{
	vec4 albedo = texture(originalAlbedo, pass_textureCoords);
	vec3 extras = texture(originalExtras, pass_textureCoords).rgb;
	out_colour.rgb = albedo.rgb * extras.g;
}
