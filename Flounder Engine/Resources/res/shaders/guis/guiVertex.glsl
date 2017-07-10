#version

//---------IN------------
layout(location = 0) in vec2 in_position;

//---------UNIFORM------------
uniform float aspectRatio;
uniform vec2 size;
uniform vec4 transform;
uniform float rotation;
uniform bool flipTexture;
uniform float atlasRows;
uniform vec2 atlasOffset;

//---------OUT------------
out vec2 pass_textureCoords;

//---------MAIN------------
void main(void) {
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
	pass_textureCoords.x = mix(pass_textureCoords.x, 1.0 - pass_textureCoords.x, flipTexture);
}
