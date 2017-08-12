#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UBO 
{
	float shineDamper;
	float reflectivity;
} ubo;

layout(location = 0) flat in vec3 surfaceNormal;
layout(location = 1) flat in vec3 surfaceColour;

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

void main(void) 
{
	out_albedo = vec4(surfaceColour, 1.0);
	out_normals = vec4(surfaceNormal + 1.0 / 2.0, out_albedo.a);
	out_extras = vec4(ubo.shineDamper, ubo.reflectivity, 0.0, out_albedo.a);
}