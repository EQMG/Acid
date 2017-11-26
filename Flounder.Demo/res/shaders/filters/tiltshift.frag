#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerColour;

layout(binding = 1) uniform UBO 
{
	float blurAmount;
	float centre;
	float stepSize;
	float steps;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	// Work out how much to blur based on the mid point.
	float amount = pow((fragmentUv.y * ubo.centre) * 2.0 - 1.0, 2.0) * ubo.blurAmount;
	float offsetMin = (float(ubo.steps - 1.0)) / -2.0;
	float offsetMax = (float(ubo.steps - 1.0)) / +2.0;
		
	// This is the accumulation of color from the surrounding pixels in the texture.
	outColour = vec4(0.0, 0.0, 0.0, 1.0);

	// From minimum offset to maximum offset.
	for (float offsetX = offsetMin; offsetX <= offsetMax; ++offsetX) 
	{
		for (float offsetY = offsetMin; offsetY <= offsetMax; ++offsetY) 
		{
			// Copy the coord so we can mess with it.
			vec2 tempfragmentUv = fragmentUv.xy;

			// Work out which uv we want to sample now.
			tempfragmentUv.x += offsetX * amount * ubo.stepSize;
			tempfragmentUv.y += offsetY * amount * ubo.stepSize;

			// Accumulate the sample
			outColour += texture(samplerColour, tempfragmentUv);
		}
	}
		
	// Because we are doing an average, we divide by the amount (x AND y, hence steps * steps).
	outColour /= float(ubo.steps * ubo.steps);
}
