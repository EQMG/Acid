//---------IN------------
attribute highp vec3 in_position;
attribute highp vec2 in_textureCoords;

//---------OUT------------
varying highp vec2 pass_textureCoords;

//---------MAIN------------
void main(void) 
{
	pass_textureCoords = vec2(1.0 - in_textureCoords.x, in_textureCoords.y);
	gl_Position = vec4(in_position, 1.0);
}
