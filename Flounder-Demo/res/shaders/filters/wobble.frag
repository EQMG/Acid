#version 450
#extension GL_ARB_separate_shader_objects : enable

const float DI = 1.0 / 64.0;

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float moveIt;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

//---------MAIN------------
void main(void)
{
	vec2 t0 = textureCoords;
	t0.x += cos(2.0 * 3.14 * textureCoords.y * 4.0 + ubo.moveIt) * DI;
	t0.y += sin(2.0 * 3.14 * textureCoords.x * 4.0 + ubo.moveIt) * DI;
	outColour = texture(samplerTexture, t0);
}
