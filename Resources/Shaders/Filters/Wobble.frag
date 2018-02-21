#version 450
#extension GL_ARB_separate_shader_objects : enable

const float DI = 1.0 / 64.0;
#define PI 3.141592653589793f

layout(set = 0, binding = 0) uniform UboScene 
{
	float moveIt;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 2) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

//---------MAIN------------
void main()
{
	vec2 t0 = fragmentUv;
	t0.x += cos(2.0 * PI * fragmentUv.y * 4.0 + scene.moveIt) * DI;
	t0.y += sin(2.0 * PI * fragmentUv.x * 4.0 + scene.moveIt) * DI;
	outColour = texture(samplerColour, t0);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
