//---------IN------------
attribute highp vec2 in_position;

//---------UNIFORM------------
uniform highp float aspectRatio;
uniform highp vec2 size;
uniform highp vec4 transform;
uniform highp float rotation;
uniform bool flipTexture;
uniform highp float atlasRows;
uniform highp vec2 atlasOffset;

//---------OUT------------
varying highp vec2 pass_textureCoords;

//---------MAIN------------
void main(void) 
{
	vec2 screenPosition = vec2(
		(in_position.x - size.x) * transform.z * cos(rotation) - (in_position.y - size.y) * transform.w * sin(rotation),
		(in_position.x - size.x) * transform.z * sin(rotation) + (in_position.y - size.y) * transform.w * cos(rotation)
	);
	screenPosition = screenPosition + transform.xy;
	screenPosition.x = (screenPosition.x / aspectRatio) * 2.0 - 1.0;
	screenPosition.y = screenPosition.y * -2.0 + 1.0;
	gl_Position = vec4(screenPosition, 0.0, 1.0);

	pass_textureCoords = in_position;
	pass_textureCoords = (pass_textureCoords / atlasRows) + atlasOffset;

	if (flipTexture)
	{
		pass_textureCoords.x = 1.0 - pass_textureCoords.x;
	}
}
