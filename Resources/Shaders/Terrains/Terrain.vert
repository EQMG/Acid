#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
} scene;

layout(binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec2 outUv;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	vec4 worldPosition = object.transform * vec4(inPosition, 1.0f);
    mat3 normalMatrix = transpose(inverse(mat3(object.transform)));

	gl_Position = scene.projection * scene.view * worldPosition;
	
	outPosition = worldPosition;
	outUv = inUv;
	outNormal = normalMatrix * normalize(inNormal);
	outTangent = inTangent;
}
