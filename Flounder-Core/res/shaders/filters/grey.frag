#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec3 colour = texture(samplerTexture, textureCoords).rgb;
	float grey = dot(colour, vec3(0.299, 0.587, 0.114));
	outColour = vec4(grey, grey, grey, 1.0);
}
