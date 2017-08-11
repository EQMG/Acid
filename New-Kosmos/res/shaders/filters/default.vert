#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;

out vec2 pass_textureCoords;

void main(void) 
{
	pass_textureCoords = vec2(1.0 - in_textureCoords.x, in_textureCoords.y);
	gl_Position = vec4(in_position, 1.0);
}
