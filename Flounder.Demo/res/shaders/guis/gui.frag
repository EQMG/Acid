#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	vec4 scissor;
	vec2 size;
	vec4 transform;
	float rotation;
	float alpha;
	float flipTexture;
	float atlasRows;
	vec2 atlasOffset;
	vec4 colourOffset;
} object;

layout(binding = 2) uniform sampler2D samplerTexture;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = texture(samplerTexture, fragmentTextures) + vec4(object.colourOffset.rgb, 0.0);
	outColour.a *= object.alpha;

	if (outColour.a < 0.05)
	{
		outColour = vec4(0.0);
		discard;
	}
}
