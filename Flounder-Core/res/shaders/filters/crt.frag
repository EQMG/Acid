#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	vec3 screenColour;
	float curveAmountX;
	float curveAmountY;
	float scanLineSize;
	float scanIntensity;
	float moveTime;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec2 tc = vec2(textureCoords.x, textureCoords.y);

	// Distance from the center
	float dx = abs(0.5 - tc.x);
	float dy = abs(0.5 - tc.y);

	// Square it to smooth the edges
	dx *= dx;
	dy *= dy;

	tc.x -= 0.5;
	tc.x *= 1.0 + (dy * ubo.curveAmountX);
	tc.x += 0.5;

	tc.y -= 0.5;
	tc.y *= 1.0 + (dx * ubo.curveAmountY);
	tc.y += 0.5;

	// Get texel, and add in scanline if need be
	vec4 colour = texture(samplerTexture, tc);
	colour.rgb += sin((tc.y + ubo.moveTime) * ubo.scanLineSize) * ubo.scanIntensity;

	// Cutoff
	if (tc.y > 1.0 || tc.x < 0.0 || tc.x > 1.0 || tc.y < 0.0) 
	{
		colour = vec4(0.0);
	}

	float grey = dot(colour.xyz, vec3(0.299, 0.587, 0.114));
	outColour = vec4(ubo.screenColour * grey, 1.0);
}
