#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float factor;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec3 colour = texture(samplerTexture, textureCoords).rgb;
	outColour = vec4(colour * ubo.factor, 1.0);
}
