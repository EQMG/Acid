#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboObject
{
	vec4 transform;
	vec4 colourOffset;
	vec2 atlasOffset;
	float atlasRows;
	float alpha;
} object;

layout(set = 0, binding = 1) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main() 
{
	outAlbedo = texture(samplerAlbedo, inUv) * vec4(object.colourOffset.rgb, 1.0f);
	outAlbedo.a *= object.alpha;

	if (outAlbedo.a < 0.05f)
	{
		outAlbedo = vec4(0.0f);
		discard;
	}
}
