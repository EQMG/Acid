#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	vec4 screenColour;
	float curveAmountX;
	float curveAmountY;
	float scanLineSize;
	float scanIntensity;
	float moveTime;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 2) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main() 
{
	vec2 tc = vec2(inUv.x, inUv.y);

	// Distance from the center
	float dx = abs(0.5f - tc.x);
	float dy = abs(0.5f - tc.y);

	// Square it to smooth the edges
	dx *= dx;
	dy *= dy;

	tc.x -= 0.5f;
	tc.x *= 1.0f + (dy * scene.curveAmountX);
	tc.x += 0.5f;

	tc.y -= 0.5f;
	tc.y *= 1.0f + (dx * scene.curveAmountY);
	tc.y += 0.5f;

	// Get texel, and add in scanline if need be
	vec4 textureColour = texture(samplerAlbedo, tc);
	textureColour.rgb += sin((tc.y + scene.moveTime) * scene.scanLineSize) * scene.scanIntensity;

	// Cutoff
	if (tc.y > 1.0f || tc.x < 0.0f || tc.x > 1.0f || tc.y < 0.0f)
	{
		textureColour = vec4(0.0f);
	}

	float grey = dot(textureColour.xyz, vec3(0.299f, 0.587f, 0.114f));
	outAlbedo = vec4(scene.screenColour.rgb * grey, 1.0f);

	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
