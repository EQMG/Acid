#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float spanMax;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

const float reduceMin = 1.0f / 128.0f;
const float reduceMul = 1.0f / 8.0f;

void main() 
{
	ivec2 originalSize = textureSize(samplerColour, 0);
	vec2 resolution = vec2(originalSize.x, originalSize.y);

	vec2 inverseResolution = vec2(1.0f / resolution.x, 1.0f / resolution.y);
	vec3 rgbNW = texture(samplerColour, inUv + vec2(-1.0f, -1.0f) * inverseResolution).rgb;
	vec3 rgbNE = texture(samplerColour, inUv + vec2(1.0f, -1.0f) * inverseResolution).rgb;
	vec3 rgbSW = texture(samplerColour, inUv + vec2(-1.0f, 1.0f) * inverseResolution).rgb;
	vec3 rgbSE = texture(samplerColour, inUv + vec2(1.0f, 1.0f) * inverseResolution).rgb;
	vec3 rgbM = texture(samplerColour, inUv * inverseResolution).rgb;

	vec3 luma = vec3(0.299f, 0.587f, 0.114f);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM = dot(rgbM, luma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25f * reduceMul), reduceMin);

	float rcpDirMin = 1.0f / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(scene.spanMax, scene.spanMax), max(vec2(-scene.spanMax, -scene.spanMax), dir * rcpDirMin)) * inverseResolution;

  	vec3 rgbA = (1.0f / 2.0f) * (texture(samplerColour, inUv.xy + dir * (1.0f / 3.0f - 0.5f)).xyz + texture(samplerColour, inUv.xy + dir * (2.0f / 3.0f - 0.5f)).xyz);
  	vec3 rgbB = rgbA * (1.0f / 2.0f) + (1.0f / 4.0f) * (texture(samplerColour, inUv.xy + dir * (0.0f / 3.0f - 0.5f)).xyz + texture(samplerColour, inUv.xy + dir * (3.0f / 3.0f - 0.5f)).xyz);
  	float lumaB = dot(rgbB, luma);

	if ((lumaB < lumaMin) || (lumaB > lumaMax)) 
	{
		outColour = vec4(rgbA, 1.0f);
	} 
	else
	{
		outColour = vec4(rgbB, 1.0f);
	}
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
