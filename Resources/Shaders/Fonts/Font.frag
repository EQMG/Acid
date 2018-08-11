#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboObject
{
	vec4 transform;
	vec4 colour;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
	float alpha;
} object;

layout(set = 0, binding = 1) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main() 
{
	float distance = texture(samplerAlbedo, inUv).a;
	float alpha = smoothstep((1.0f - object.edgeData.x) - object.edgeData.y, 1.0f - object.edgeData.x, distance);
	float outlineAlpha = smoothstep((1.0f - object.borderSizes.x) - object.borderSizes.y, 1.0f - object.borderSizes.x, distance);
	float overallAlpha = alpha + (1.0f - alpha) * outlineAlpha;
	vec3 overallColour = mix(object.borderColour.rgb, object.colour.rgb, alpha / overallAlpha);

	outAlbedo = vec4(overallColour, overallAlpha);
	outAlbedo.a *= object.alpha;

	if (outAlbedo.a < 0.05f)
	{
		outAlbedo = vec4(0.0f);
		discard;
	}
}
