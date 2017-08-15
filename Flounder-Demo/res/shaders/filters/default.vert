#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoords;

layout(location = 0) out vec2 textureCoords;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main(void) 
{
	textureCoords = vec2(1.0 - inTextureCoords.x, inTextureCoords.y);
	gl_Position = vec4(inPosition, 1.0);
}
