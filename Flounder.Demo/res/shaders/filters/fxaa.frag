#version 450
#extension GL_ARB_separate_shader_objects : enable

const float FXAA_REDUCE_MIN = (1.0 / 128.0);
const float FXAA_REDUCE_MUL = (1.0 / 8.0);

layout(set = 0, binding = 0) uniform sampler2D samplerColour;

layout(set = 0, binding = 1) uniform UBO 
{
	float spanMax;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	ivec2 originalSize = textureSize(samplerColour, 0);
	vec2 resolution = vec2(originalSize.x, originalSize.y);

	vec2 inverseResolution = vec2(1.0 / resolution.x, 1.0 / resolution.y);
	vec3 rgbNW = texture(samplerColour, fragmentUv + vec2(-1.0, -1.0) * inverseResolution).rgb;
	vec3 rgbNE = texture(samplerColour, fragmentUv + vec2(1.0, -1.0) * inverseResolution).rgb;
	vec3 rgbSW = texture(samplerColour, fragmentUv + vec2(-1.0, 1.0) * inverseResolution).rgb;
	vec3 rgbSE = texture(samplerColour, fragmentUv + vec2(1.0, 1.0) * inverseResolution).rgb;
	vec3 rgbM = texture(samplerColour, fragmentUv * inverseResolution).rgb;

	vec3 luma = vec3(0.299, 0.587, 0.114);
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

	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(ubo.spanMax,ubo.spanMax), max(vec2(-ubo.spanMax, -ubo.spanMax), dir * rcpDirMin)) * inverseResolution;

  	vec3 rgbA = (1.0/2.0) * (texture(samplerColour, fragmentUv.xy + dir * (1.0/3.0 - 0.5)).xyz + texture(samplerColour, fragmentUv.xy + dir * (2.0/3.0 - 0.5)).xyz);
  	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (texture(samplerColour, fragmentUv.xy + dir * (0.0/3.0 - 0.5)).xyz + texture(samplerColour, fragmentUv.xy + dir * (3.0/3.0 - 0.5)).xyz);
  	float lumaB = dot(rgbB, luma);

	if ((lumaB < lumaMin) || (lumaB > lumaMax)) 
	{
		outColour = vec4(rgbA, 1.0);
	} 
	else
	{
		outColour = vec4(rgbB, 1.0);
	}
}
