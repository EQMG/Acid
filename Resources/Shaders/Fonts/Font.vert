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
	vec4 worldPosition = object.worldTransform * position;

	if (object.worldTransform != mat4(1.0f))
	{
		gl_Position = scene.projection * scene.view * worldPosition;
	}
	else
	{
		gl_Position = position;
	}

	outUv = inUv;
}
