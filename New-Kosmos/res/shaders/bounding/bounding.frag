#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec4 pass_positionRelativeToCam;
in vec3 pass_surfaceNormal;

uniform vec3 colour;

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;
layout(location = 2) out vec4 out_extras;

void main(void) 
{
	out_albedo = vec4(colour, 1.0);
	out_normals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 1.0);
	out_extras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}
