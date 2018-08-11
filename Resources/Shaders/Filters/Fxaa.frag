#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float spanMax;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 2) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

const float reduceMin = 1.0f / 128.0f;
const float reduceMul = 1.0f / 8.0f;

void main() 
{
	ivec2 originalSize = textureSize(samplerAlbedo, 0);
	vec2 resolution = vec2(originalSize.x, originalSize.y);

	vec2 inverseResolution = vec2(1.0f / resolution.x, 1.0f / resolution.y);
	vec3 rgbNW = texture(samplerAlbedo, inUv + vec2(-1.0f, -1.0f) * inverseResolution).rgb;
	vec3 rgbNE = texture(samplerAlbedo, inUv + vec2(1.0f, -1.0f) * inverseResolution).rgb;
	vec3 rgbSW = texture(samplerAlbedo, inUv + vec2(-1.0f, 1.0f) * inverseResolution).rgb;
	vec3 rgbSE = texture(samplerAlbedo, inUv + vec2(1.0f, 1.0f) * inverseResolution).rgb;
	vec3 rgbM = texture(samplerAlbedo, inUv * inverseResolution).rgb;

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

  	vec3 rgbA = (1.0f / 2.0f) * (texture(samplerAlbedo, inUv.xy + dir * (1.0f / 3.0f - 0.5f)).xyz + texture(samplerAlbedo, inUv.xy + dir * (2.0f / 3.0f - 0.5f)).xyz);
  	vec3 rgbB = rgbA * (1.0f / 2.0f) + (1.0f / 4.0f) * (texture(samplerAlbedo, inUv.xy + dir * (0.0f / 3.0f - 0.5f)).xyz + texture(samplerAlbedo, inUv.xy + dir * (3.0f / 3.0f - 0.5f)).xyz);
  	float lumaB = dot(rgbB, luma);

	if ((lumaB < lumaMin) || (lumaB > lumaMax)) 
	{
		outAlbedo = vec4(rgbA, 1.0f);
	} 
	else
	{
		outAlbedo = vec4(rgbB, 1.0f);
	}
	
	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
