#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float spanMax;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------CONSTANT------------
const float FXAA_REDUCE_MIN = (1.0 / 128.0);
const float FXAA_REDUCE_MUL = (1.0 / 8.0);

//---------APPLY FXAA------------
vec4 applyFXAA(sampler2D original, vec2 fragCoord, vec2 resolution) {
	vec2 inverseResolution = vec2(1.0 / resolution.x, 1.0 / resolution.y);
	vec3 rgbNW = texture(original, fragCoord + vec2(-1.0, -1.0) * inverseResolution).rgb;
	vec3 rgbNE = texture(original, fragCoord + vec2(1.0, -1.0) * inverseResolution).rgb;
	vec3 rgbSW = texture(original, fragCoord + vec2(-1.0, 1.0) * inverseResolution).rgb;
	vec3 rgbSE = texture(original, fragCoord + vec2(1.0, 1.0) * inverseResolution).rgb;
	vec3 rgbM = texture(original, fragCoord * inverseResolution).rgb;

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

	dir = min(vec2(spanMax,  spanMax), max(vec2(-spanMax, -spanMax), dir * rcpDirMin)) * inverseResolution;

  	vec3 rgbA = (1.0/2.0) * (texture(original, fragCoord.xy + dir * (1.0/3.0 - 0.5)).xyz + texture(original, fragCoord.xy + dir * (2.0/3.0 - 0.5)).xyz);
  	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (texture(original, fragCoord.xy + dir * (0.0/3.0 - 0.5)).xyz + texture(original, fragCoord.xy + dir * (3.0/3.0 - 0.5)).xyz);
  	float lumaB = dot(rgbB, luma);

	if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
		return vec4(rgbA, 1.0);
	} else {
		return vec4(rgbB, 1.0);
	}
}

//---------MAIN------------
void main(void) {
	ivec2 originalSize = textureSize(originalTexture, 0);
	out_colour = applyFXAA(originalTexture, pass_textureCoords, vec2(originalSize.x, originalSize.y));
}
