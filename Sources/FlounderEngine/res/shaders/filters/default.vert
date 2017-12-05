#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 infragmentUv;

layout(location = 0) out vec2 fragmentUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	fragmentUv = vec2(infragmentUv.x, infragmentUv.y);
	gl_Position = vec4(inPosition, 1.0);
}
