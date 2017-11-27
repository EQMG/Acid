#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float blurAmount;
	float centre;
	float stepSize;
	float steps;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	// Work out how much to blur based on the mid point.
	float amount = pow((fragmentUv.y * scene.centre) * 2.0 - 1.0, 2.0) * scene.blurAmount;
	float offsetMin = (float(scene.steps - 1.0)) / -2.0;
	float offsetMax = (float(scene.steps - 1.0)) / +2.0;
		
	// This is the accumulation of color from the surrounding pixels in the texture.
	outColour = vec4(0.0, 0.0, 0.0, 1.0);

	// From minimum offset to maximum offset.
	for (float offsetX = offsetMin; offsetX <= offsetMax; ++offsetX) 
	{
		for (float offsetY = offsetMin; offsetY <= offsetMax; ++offsetY) 
		{
			// Copy the coord so we can mess with it.
			vec2 tempUv = fragmentUv.xy;

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
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
