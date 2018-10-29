#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 modelMatrix;
	vec4 screenOffset;
	vec4 colourOffset;
	vec2 atlasOffset;
	float atlasRows;
	float alpha;
	bool lockRotation;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	outColour = texture(samplerColour, inUv) * vec4(object.colourOffset.rgb, 1.0f);
	outColour.a *= object.alpha;

	if (outColour.a < 0.05f)
	{
		outColour = vec4(0.0f);
		discard;
	}
}
