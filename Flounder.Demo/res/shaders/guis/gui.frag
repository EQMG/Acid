#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboObject
{
	vec4 transform;
	vec4 colourOffset;
	vec2 atlasOffset;
	float atlasRows;
	float alpha;
} object;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = texture(samplerColour, fragmentUv) * vec4(object.colourOffset.rgb, 1.0f);
	outColour.a *= object.alpha;

	if (outColour.a < 0.05f)
	{
		outColour = vec4(0.0f);
		discard;
	}
}
