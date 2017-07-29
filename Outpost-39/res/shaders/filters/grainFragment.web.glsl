precision highp float;

//---------IN------------
varying highp vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform highp float strength;

//---------MAIN------------
void main(void) 
{
	vec4 colour = texture2D(originalTexture, pass_textureCoords);

    float x = (pass_textureCoords.x + 4.0) * (pass_textureCoords.y + 4.0) * 10.0;
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01) - 0.005) * strength;

	gl_FragColor = colour + grain;
}
