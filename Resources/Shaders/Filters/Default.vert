#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;

layout(location = 0) out vec2 fragmentUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = vec4(vertexPosition, 1.0);

	fragmentUv = vertexUv;
}
