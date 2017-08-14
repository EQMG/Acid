#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UBO 
{
	mat4 mvpMatrix;
} ubo;

layout(location = 0) in vec3 inPosition;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main(void) 
{
	gl_Position = ubo.mvpMatrix * vec4(inPosition, 1.0);
}