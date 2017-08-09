#version

//---------IN------------
layout(location = 0) in vec3 in_position;

//---------UNIFORM------------
uniform mat4 mvpMatrix;

//---------MAIN------------
void main(void) 
{
	gl_Position = mvpMatrix * vec4(in_position, 1.0);
}