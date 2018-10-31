#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float maxBlurSize;
	float radiusScale;
	float goldenAngle;

	float nearPlane;
	float farPlane;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerDepth;
layout(set = 0, binding = 3) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

const float uFocusPoint = 4.0f; // Distance from camera.
const float uFocusScale = 8.0f; // Scale of blur.

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0f * scene.nearPlane * scene.farPlane) / (scene.farPlane + scene.nearPlane - z * (scene.farPlane - scene.nearPlane));
}

float getBlurSize(float depth, float focusPoint, float focusScale)
{
	float coc = clamp((1.0f / focusPoint - 1.0f / depth) * focusScale, -1.0f, 1.0f);
	return abs(coc) * scene.maxBlurSize;
}

void main() 
{
	vec2 sizeColour = textureSize(samplerColour, 0);
	vec2 pixelSize = 1.0f / sizeColour;

	float centerDepth = linearDepth(texture(samplerDepth, inUv).r);
	float centerSize = getBlurSize(centerDepth, uFocusPoint, uFocusScale);
	vec3 colour = texture(samplerColour, inUv).rgb;
	float tot = 1.0f;

	float radius = scene.radiusScale;

	for (float ang = 0.0f; radius < scene.maxBlurSize; ang += scene.goldenAngle)
	{
	    vec2 tc = inUv + vec2(cos(ang), sin(ang)) * pixelSize * radius;

		vec3 sampleColor = texture(samplerColour, tc).rgb;
		float sampleDepth = linearDepth(texture(samplerDepth, tc).r);
		float sampleSize = getBlurSize(sampleDepth, uFocusPoint, uFocusScale);

		if (sampleDepth > centerDepth)
		{
			sampleSize = clamp(sampleSize, 0.0f, centerSize * 2.0f);
		}

		float m = smoothstep(radius - 0.5f, radius + 0.5f, sampleSize);
		colour += mix(colour / tot, sampleColor, m);
		tot += 1.0f;
		radius += scene.radiusScale / radius;
	}

	outColour = vec4(colour /= tot, 1.0f);

	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
