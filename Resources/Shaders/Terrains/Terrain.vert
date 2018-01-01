#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

flat layout(location = 0) out vec3 fragmentNormal;
layout(location = 1) out vec2 fragmentUv;
layout(location = 2) out vec3 fragmentColour;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main() 
{
	vec4 totalLocalPos = vec4(vertexPosition, 1.0f);
	vec4 totalNormal = vec4(vertexNormal, 0.0f);

	vec4 worldPosition = object.transform * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, scene.clip);
	gl_Position = scene.projection * scene.view * worldPosition;

	fragmentNormal = normalize((object.transform * totalNormal).xyz);
	fragmentUv = vertexUv;
	fragmentColour = vertexTangent;
}
