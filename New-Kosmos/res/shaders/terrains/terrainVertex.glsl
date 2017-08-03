#version

//---------IN------------
layout(location = 0) in vec3 in_position;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_colour;

//---------UNIFORM------------
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 clipPlane;
uniform mat4 modelMatrix;

//---------OUT------------
flat out vec3 pass_normal;
flat out vec3 pass_colour;

//---------MAIN------------
void main(void) {
	vec4 totalLocalPos = vec4(in_position, 1.0);
	vec4 totalNormal = vec4(in_normal, 0.0);

	vec4 worldPosition = modelMatrix * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	pass_normal = normalize((modelMatrix * totalNormal).xyz);
	pass_colour = vec3(in_colour);
}
