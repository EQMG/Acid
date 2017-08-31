#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float aspectRatio;
	vec2 size;
	vec4 transform;
	float rotation;
	bool flipTexture;
	float atlasRows;
	vec2 atlasOffset;

	float alpha;
	vec3 colourOffset;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = texture(samplerTexture, textureCoords) + vec4(ubo.colourOffset, 0.0);
	outColour.a *= ubo.alpha;

	if (outColour.a < 0.05)
	{
		outColour = vec4(0.0);
		discard;
	}
}
