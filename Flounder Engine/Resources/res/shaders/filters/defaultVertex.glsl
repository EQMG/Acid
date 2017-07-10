#version

//---------IN------------
layout(location = 0) in vec2 position;

//---------OUT------------
out vec2 pass_textureCoords;

//---------MAIN------------
void main(void) {
	pass_textureCoords = vec2(position.x, 1.0 - position.y);
	gl_Position = vec4(vec2(position.x * 2.0 - 1.0, position.y * -2.0 + 1.0), 0.0, 1.0);
}
