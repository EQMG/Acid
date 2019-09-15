#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UniformObject {
	mat4 modelView;
	float alpha;
	
	float aspectRatio;

	vec4 colourOffset;
	vec2 atlasOffset;
	vec2 atlasScale;
	float atlasRows;
	vec4 ninePatches;
} object;

layout(binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

float map(float value, float originalMin, float originalMax, float newMin, float newMax) {
	return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

float processAxis(float coord, float textureBorder, float windowBorder) {
	if (coord < windowBorder) {
		return map(coord, 0.0f, windowBorder, 0.0f, textureBorder);
	}

	if (coord < 1.0f - windowBorder) {
		return map(coord, windowBorder, 1.0f - windowBorder, textureBorder, 1.0f - textureBorder);
	}

	return map(coord, 1.0f - windowBorder, 1.0f, 1.0f - textureBorder, 1.0f);
}

void main() {
	if (object.ninePatches != vec4(0.0f)) {
		vec2 newUV = vec2(
		    processAxis(inUV.x, object.ninePatches.x, object.ninePatches.x / object.aspectRatio),
		    processAxis(inUV.y, object.ninePatches.y, object.ninePatches.y)
		);

		outColour = texture(samplerColour, newUV);
	} else {
		outColour = texture(samplerColour, inUV);
	}

	outColour *= object.colourOffset;
	outColour.a *= object.alpha;

	if (outColour.a < 0.05f) {
		outColour = vec4(0.0f);
		discard;
	}
}
