#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene
{
	float moveIt;
} scene;

layout(set = 0, binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

const float pi = 3.141592653589793238462643f;
const float di = 1.0f / 64.0f;

void main()
{
	vec2 t0 = inUv;
	t0.x += cos(2.0f * pi * inUv.y * 4.0f + scene.moveIt) * di;
	t0.y += sin(2.0f * pi * inUv.x * 4.0f + scene.moveIt) * di;
	vec4 colour = texture(samplerColour, t0);
	
	imageStore(writeColour, ivec2(inUv * imageSize(writeColour)), colour);
}
