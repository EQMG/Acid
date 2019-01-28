#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 modelMatrix;
	vec2 screenDimension;
	vec2 screenPosition;
	float aspectRatio;
	float alpha;
	float depth;
	int modelMode;

	vec4 colourOffset;
	vec2 atlasOffset;
	float atlasRows;
	vec4 ninePatches;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

float map(float value, float originalMin, float originalMax, float newMin, float newMax)
{
	return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

float processAxis(float coord, float textureBorder, float windowBorder)
{
	if (coord < windowBorder)
	{
		return map(coord, 0.0f, windowBorder, 0.0f, textureBorder);
	}

	if (coord < 1.0f - windowBorder)
	{
		return map(coord,  windowBorder, 1.0f - windowBorder, textureBorder, 1.0f - textureBorder);
	}

	return map(coord, 1.0f - windowBorder, 1.0f, 1.0f - textureBorder, 1.0f);
}

void main() 
{
	if (object.ninePatches != vec4(0.0f))
	{
		vec2 newUV = vec2(
		    processAxis(inUv.x, object.ninePatches.x, object.ninePatches.x / (object.screenDimension.x / object.screenDimension.y) / object.aspectRatio),
		    processAxis(inUv.y, object.ninePatches.y, object.ninePatches.y)
		);

		outColour = texture(samplerColour, newUV);
		/*outColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		if (newUV.x < object.ninePatches.x || newUV.x > object.ninePatches.z)
		{
			outColour.r = 1.0f;
		}

		if (newUV.y < object.ninePatches.y || newUV.y > object.ninePatches.w)
		{
			outColour.g = 1.0f;
		}*/
	}
	else
	{
		outColour = texture(samplerColour, inUv);
	}

	outColour *= object.colourOffset;
	outColour.a *= object.alpha;

	if (outColour.a < 0.05f)
	{
		outColour = vec4(0.0f);
		discard;
	}
}
