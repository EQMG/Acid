#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

struct Instance
{
	mat4 transform;
	vec4 diffuse;
};

layout(set = 0, binding = 1) buffer Instances
{
	Instance data[];
} instances;

layout(set = 0, location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outDiffuse;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	Instance instance = instances.data[gl_InstanceIndex];

	vec4 worldPosition = instance.transform * vec4(inPosition, 1.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

	outDiffuse = instance.diffuse;
}
