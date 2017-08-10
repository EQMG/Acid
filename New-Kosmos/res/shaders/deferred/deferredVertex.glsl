#version 150 core

//---------IN------------
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;

//---------OUT------------
out vec2 pass_textureCoords;

//---------MAIN------------
void main(void) 
{
	pass_textureCoords = vec2(1 - in_textureCoords.x, in_textureCoords.y);
	gl_Position = vec4(in_position, 1.0);
}
