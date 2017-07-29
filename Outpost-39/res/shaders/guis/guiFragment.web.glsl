precision highp float;

//---------IN------------
varying highp vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D guiTexture;
uniform bool polygonMode;
uniform highp float alpha;
uniform highp vec4 colourOffset;

//---------MAIN------------
void main(void) 
{
	gl_FragColor = texture2D(guiTexture, pass_textureCoords) + vec4(colourOffset.rgb, 0.0);
	gl_FragColor.a *= alpha;

	if (polygonMode) 
	{
		gl_FragColor = vec4(1.0, 0.0, 0.0, alpha);
	}

	if (gl_FragColor.a < 0.05)
	{
		gl_FragColor = vec4(0.0);
		discard;
	}
}
