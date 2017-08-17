#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float strength;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec4 colour = texture(samplerTexture, textureCoords);

	float x = (textureCoords.x + 4.0) * (textureCoords.y + 4.0) * 10.0;
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01) - 0.005) * ubo.strength;

	outColour = colour + grain;
}
