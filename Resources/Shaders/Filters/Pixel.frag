#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float pixelSize;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec2 sizeColour = textureSize(samplerColour, 0);
	vec2 coord;

	if (inUv.x < 1.0f)
	{
		float dx = scene.pixelSize * (1.0f / sizeColour.x);
		float dy = scene.pixelSize * (1.0f / sizeColour.y);
		coord.x = dx * floor(inUv.x / dx);
		coord.y = dy * floor(inUv.y / dy);
	} 
	else if (inUv.x > 1.0)
	{
		coord = inUv;
	}

	vec3 colour = texture(samplerColour, coord).rgb;
	outColour = vec4(colour, 1.0f);

	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
