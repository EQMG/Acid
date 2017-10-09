#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} scene;

layout(binding = 1) uniform UboObject
{
	mat4 transform;

	vec4 diffuseColour;

	float shineDamper;
	float reflectivity;

	float ignoreReflections;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextures;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(location = 0) out vec3 fragmentNormal;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main(void) 
{
	vec4 totalLocalPos = vec4(vertexPosition, 1.0);
	vec4 totalNormal = vec4(vertexNormal, 0.0);

	vec4 worldPosition = object.transform * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, scene.clip);
	gl_Position = scene.projection * scene.view * worldPosition;

	fragmentNormal = normalize((object.transform * totalNormal).xyz);
}
