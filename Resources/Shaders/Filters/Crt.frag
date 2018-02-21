#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	vec4 screenColour;
	float curveAmountX;
	float curveAmountY;
	float scanLineSize;
	float scanIntensity;
	float moveTime;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 2) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec2 tc = vec2(fragmentUv.x, fragmentUv.y);

	// Distance from the center
	float dx = abs(0.5 - tc.x);
	float dy = abs(0.5 - tc.y);

	// Square it to smooth the edges
	dx *= dx;
	dy *= dy;

	tc.x -= 0.5;
	tc.x *= 1.0 + (dy * scene.curveAmountX);
	tc.x += 0.5;

	tc.y -= 0.5;
	tc.y *= 1.0 + (dx * scene.curveAmountY);
	tc.y += 0.5;

	// Get texel, and add in scanline if need be
	vec4 textureColour = texture(samplerColour, tc);
	textureColour.rgb += sin((tc.y + scene.moveTime) * scene.scanLineSize) * scene.scanIntensity;

	// Cutoff
	if (tc.y > 1.0 || tc.x < 0.0 || tc.x > 1.0 || tc.y < 0.0) 
	{
		textureColour = vec4(0.0);
	}

	float grey = dot(textureColour.xyz, vec3(0.299, 0.587, 0.114));
	outColour = vec4(scene.screenColour.rgb * grey, 1.0);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
