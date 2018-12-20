#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 modelMatrix;
	vec4 screenOffset;
	vec4 colourOffset;
	vec2 atlasOffset;
	float atlasRows;
	float alpha;
	float depth;
	int modelMode;
} object;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec2 inUv;

layout(location = 0) out vec2 outUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

#include "Shaders/Billboard.glsl"
const vec3 rotation = vec3(3.14159f, 0.0f, 0.0f);

void main()
{
	vec4 position = vec4((inPosition.xy * object.screenOffset.xy) + object.screenOffset.zw, 0.0f, 1.0f);

	if (object.modelMode != 0)
	{
		mat4 modelMatrix = modelMatrix(object.modelMatrix, scene.view, object.modelMode == 2, rotation);
		vec4 worldPosition = modelMatrix * position;
		gl_Position = scene.projection * scene.view * worldPosition;
	}
	else
	{
		gl_Position = position;
		gl_Position.z = 0.5f;
	}

	gl_Position.z -= 0.01f * object.depth;

	outUv = (inUv.xy / object.atlasRows) + object.atlasOffset;
}
