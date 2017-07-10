#version

//---------IN------------
layout(location = 0) in vec3 in_position;

//---------UNIFORM------------
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 clipPlane;

//---------OUT------------
out vec3 pass_textureCoords;

//---------MAIN------------
void main(void) {
	vec4 worldPosition = modelMatrix * vec4(in_position, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	pass_textureCoords = in_position;
}
