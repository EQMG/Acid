#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 clipPlane;
	mat4 modelMatrix;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inColour;

layout(location = 0) flat out vec3 surfaceNormal;
layout(location = 1) flat out vec3 surfaceColour;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main(void) 
{
	vec4 totalLocalPos = vec4(inPosition, 1.0);
	vec4 totalNormal = vec4(inNormal, 0.0);

	vec4 worldPosition = ubo.modelMatrix * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, ubo.clipPlane);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;

	surfaceNormal = normalize((ubo.modelMatrix * totalNormal).xyz);
	surfaceColour = vec3(inColour);
}
