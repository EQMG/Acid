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
	mat4 worldTransform;
	vec4 screenTransform;
	vec4 colour;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
	float alpha;
	bool lockRotation;
} object;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec2 inUv;

layout(location = 0) out vec2 outUv;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 position = vec4((inPosition.xy * object.screenTransform.xy) + object.screenTransform.zw, 0.0f, 1.0f);

	if (object.worldTransform != mat4(1.0f))
	{
		mat4 modelMatrix = object.worldTransform;

		if (object.lockRotation)
		{
			modelMatrix[0][0] = scene.view[0][0];
			modelMatrix[0][1] = scene.view[1][0];
			modelMatrix[0][2] = scene.view[2][0];
			modelMatrix[1][0] = scene.view[0][1];
			modelMatrix[1][1] = scene.view[1][1];
			modelMatrix[1][2] = scene.view[2][1];
			modelMatrix[2][0] = scene.view[0][2];
			modelMatrix[2][1] = scene.view[1][2];
			modelMatrix[2][2] = scene.view[2][2];
		}

		vec4 worldPosition = modelMatrix * position;
		gl_Position = scene.projection * scene.view * worldPosition;
	}
	else
	{
		gl_Position = position;
	}

	outUv = inUv;
}
