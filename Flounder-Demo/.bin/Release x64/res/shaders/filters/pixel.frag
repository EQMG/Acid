#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float pixelSize;
	vec2 displaySize;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec2 coord;

	if (textureCoords.x < 1.0) 
	{
		float dx = ubo.pixelSize * (1.0 / ubo.displaySize.x);
		float dy = ubo.pixelSize * (1.0 / ubo.displaySize.y);
		coord.x = dx * floor(textureCoords.x / dx);
		coord.y = dy * floor(textureCoords.y / dy);
	} 
	else if (textureCoords.x > 1.0) 
	{
		coord = textureCoords;
	}

	vec3 colour = texture(samplerTexture, coord).rgb;
	outColour = vec4(colour, 1.0);
}
