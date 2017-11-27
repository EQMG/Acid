#version 450
#extension GL_ARB_separate_shader_objects : enable

const float DI = 1.0 / 64.0;

layout(set = 0, binding = 0) uniform sampler2D samplerColour;

layout(set = 0, binding = 1) uniform UBO 
{
	float moveIt;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

//---------MAIN------------
void main()
{
	vec2 t0 = fragmentUv;
	t0.x += cos(2.0 * 3.14 * fragmentUv.y * 4.0 + ubo.moveIt) * DI;
	t0.y += sin(2.0 * 3.14 * fragmentUv.x * 4.0 + ubo.moveIt) * DI;
	outColour = texture(samplerColour, t0);
}
