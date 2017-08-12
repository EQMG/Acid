#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	bool polygonMode;
	vec4 colour;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	float distance = texture(samplerTexture, textureCoords).a;
	float alpha = smoothstep((1.0 - ubo.edgeData.x) - ubo.edgeData.y, 1.0 - ubo.edgeData.x, distance);
	float outlineAlpha = smoothstep((1.0 - ubo.borderSizes.x) - ubo.borderSizes.y, 1.0 - ubo.borderSizes.x, distance);
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColour = mix(ubo.borderColour.rgb, ubo.colour.rgb, alpha / overallAlpha);

	outColour = vec4(overallColour, overallAlpha);
	outColour.a *= ubo.colour.a;

	if (ubo.polygonMode) 
	{
		outColour = vec4(1.0, 0.0, 0.0, ubo.colour.a);
	}

	if (outColour.a < 0.05)
	{
		outColour = vec4(0.0);
		discard;
	}
}
