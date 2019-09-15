#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	float blurAmount;
	float centre;
	float stepSize;
	float steps;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	// Work out how much to blur based on the mid point.
	float amount = pow((inUV.y * scene.centre) * 2.0f - 1.0f, 2.0f) * scene.blurAmount;
	float offsetMin = (float(scene.steps - 1.0f)) / -2.0f;
	float offsetMax = (float(scene.steps - 1.0f)) / 2.0f;
		
	// This is the accumulation of color from the surrounding pixels in the texture.
	vec4 colour = vec4(0.0f);

	// From minimum offset to maximum offset.
	for (float offsetX = offsetMin; offsetX <= offsetMax; ++offsetX) {
		for (float offsetY = offsetMin; offsetY <= offsetMax; ++offsetY) {
			// Copy the coord so we can mess with it.
			vec2 tempUv = inUV.xy;

			// Work out which uv we want to sample now.
			tempUv.x += offsetX * amount * scene.stepSize;
			tempUv.y += offsetY * amount * scene.stepSize;

			// Accumulate the sample
			colour += texture(samplerColour, tempUv);
		}
	}
		
	// Because we are doing an average, we divide by the amount (x AND y, hence steps * steps).
	colour /= float(scene.steps * scene.steps);

	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
