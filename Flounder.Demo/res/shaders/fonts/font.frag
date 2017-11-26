#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboObject
{
	vec4 transform;
	vec4 colour;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
	float alpha;
} object;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	float distance = texture(samplerColour, fragmentUv).a;
	float alpha = smoothstep((1.0f - object.edgeData.x) - object.edgeData.y, 1.0f - object.edgeData.x, distance);
	float outlineAlpha = smoothstep((1.0f - object.borderSizes.x) - object.borderSizes.y, 1.0f - object.borderSizes.x, distance);
	float overallAlpha = alpha + (1.0f - alpha) * outlineAlpha;
	vec3 overallColour = mix(object.borderColour.rgb, object.colour.rgb, alpha / overallAlpha);

	outColour = vec4(overallColour, overallAlpha);
	outColour.a *= object.alpha; // alpha

	if (outColour.a < 0.05f)
	{
		outColour = vec4(0.0f);
		discard;
	}
}
