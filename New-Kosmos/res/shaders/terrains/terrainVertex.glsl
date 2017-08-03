#version

//---------IN------------
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;
layout(location = 2) in vec3 in_normal;

//---------UNIFORM------------
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 clipPlane;
uniform mat4 modelMatrix;

//---------OUT------------
flat out vec3 pass_surfaceNormal;

//---------MAIN------------
void main(void) {
	vec4 totalLocalPos = vec4(in_position, 1.0);
	vec4 totalNormal = vec4(in_normal, 0.0);

	vec4 worldPosition = modelMatrix * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	pass_surfaceNormal = normalize((modelMatrix * totalNormal).xyz);
}
