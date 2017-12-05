#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float pixelSize;
	vec2 displaySize;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec2 coord;

	if (fragmentUv.x < 1.0) 
	{
		float dx = scene.pixelSize * (1.0 / scene.displaySize.x);
		float dy = scene.pixelSize * (1.0 / scene.displaySize.y);
		coord.x = dx * floor(fragmentUv.x / dx);
		coord.y = dy * floor(fragmentUv.y / dy);
	} 
	else if (fragmentUv.x > 1.0) 
	{
		coord = fragmentUv;
	}

	vec3 textureColour = texture(samplerColour, coord).rgb;
	outColour = vec4(textureColour, 1.0);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
