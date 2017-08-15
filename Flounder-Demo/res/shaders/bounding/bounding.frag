#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UBO 
{
	vec3 colour;
} ubo;

layout (location = 0) out vec4 outAlbedo;
layout (location = 1) out vec4 outNormals;
layout (location = 2) out vec4 outExtras;

void main(void) 
{
	outAlbedo = vec4(ubo.colour, 1.0);
	outNormals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 1.0);
	outExtras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}
