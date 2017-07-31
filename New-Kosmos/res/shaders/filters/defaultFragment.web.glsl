precision highp float;

//---------IN------------
varying highp vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;

//---------MAIN------------
void main(void) 
{
	gl_FragColor = texture2D(originalTexture, pass_textureCoords);
}
