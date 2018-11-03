#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	float strength;
} scene;

layout(set = 0, binding = 1, rgba8) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

void main()
{
	float x = (inUv.x + 4.0f) * (inUv.y + 4.0f) * 10.0f;
	vec4 grain = vec4(mod((mod(x, 13.0f) + 1.0f) * (mod(x, 123.0f) + 1.0f), 0.01f) - 0.005f) * scene.strength;

	vec4 colour = texture(samplerColour, inUv) + grain;
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), colour);
}
