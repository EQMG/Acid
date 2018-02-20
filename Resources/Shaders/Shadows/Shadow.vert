#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboObject 
{
	mat4 mvp;
} object;

layout(location = 0) in vec3 vertexPosition;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = object.mvp * vec4(vertexPosition, 1.0f);
}