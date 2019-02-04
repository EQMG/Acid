#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UboScene
{
	mat4 projectionView;
	vec3 cameraPos;
} scene;

layout(binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 inPosition;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	vec4 worldPosition = object.transform * vec4(inPosition, 1.0f);

	gl_Position = scene.projectionView * worldPosition;
}
