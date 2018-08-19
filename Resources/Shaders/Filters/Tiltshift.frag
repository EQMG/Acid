#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float blurAmount;
	float centre;
	float stepSize;
	float steps;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	// Work out how much to blur based on the mid point.
	float amount = pow((inUv.y * scene.centre) * 2.0f - 1.0f, 2.0f) * scene.blurAmount;
	float offsetMin = (float(scene.steps - 1.0f)) / -2.0f;
	float offsetMax = (float(scene.steps - 1.0f)) / 2.0f;
		
	// This is the accumulation of color from the surrounding pixels in the texture.
	outColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// From minimum offset to maximum offset.
	for (float offsetX = offsetMin; offsetX <= offsetMax; ++offsetX) 
	{
		for (float offsetY = offsetMin; offsetY <= offsetMax; ++offsetY) 
		{
			// Copy the coord so we can mess with it.
			vec2 tempUv = inUv.xy;

			// Work out which uv we want to sample now.
			tempUv.x += offsetX * amount * scene.stepSize;
			tempUv.y += offsetY * amount * scene.stepSize;

			// Accumulate the sample
			outColour += texture(samplerColour, tempUv);
		}
	}
		
	// Because we are doing an average, we divide by the amount (x AND y, hence steps * steps).
	outColour /= float(scene.steps * scene.steps);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
