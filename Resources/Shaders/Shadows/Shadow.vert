#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projectionView;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 vertexPosition;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(vertexPosition, 1.0f);
	gl_Position = scene.projectionView * worldPosition;
}