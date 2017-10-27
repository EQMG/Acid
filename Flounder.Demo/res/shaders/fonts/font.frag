#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboObject
{
	vec4 transform;
	vec4 colour;
	float alpha;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
} object;

layout(binding = 1) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	float distance = texture(samplerTexture, fragmentTextures).a;
	float alpha = smoothstep((1.0 - object.edgeData.x) - object.edgeData.y, 1.0 - object.edgeData.x, distance);
	float outlineAlpha = smoothstep((1.0 - object.borderSizes.x) - object.borderSizes.y, 1.0 - object.borderSizes.x, distance);
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColour = mix(object.borderColour.rgb, object.colour.rgb, alpha / overallAlpha);

	/*outColour = vec4(overallColour, overallAlpha);
	outColour.a *= alpha;

	if (outColour.a < 0.05)
	{
		outColour = vec4(0.0);
		discard;
	}*/
	outColour = vec4(distance * object.colour.rgb, 1);
}
