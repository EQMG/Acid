#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
} scene;

layout(set = 0, binding = 1) uniform UboObject 
{
	mat4 transform;
	vec4 skyColour;
	vec4 fogColour;
	vec2 fogLimits;
	float blendFactor;
} object;

layout(set = 0, location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(inPosition, 1.0f);

	mat4 viewStatic = mat4(scene.view);
	viewStatic[3][0] = 0.0f;
	viewStatic[3][1] = 0.0f;
	viewStatic[3][2] = 0.0f;
	
	gl_Position = scene.projection * viewStatic * worldPosition;

	outWorldPos = worldPosition.xyz;
	outUv = inPosition;
}
