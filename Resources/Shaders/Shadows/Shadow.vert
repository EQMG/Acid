#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projectionView;
	vec3 cameraPosition;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(set = 0, location = 0) in vec3 vertexPosition;
layout(set = 0, location = 1) in vec2 vertexUv;
layout(set = 0, location = 2) in vec3 vertexNormal;
layout(set = 0, location = 3) in vec3 vertexTangent;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(vertexPosition, 1.0f);

	gl_Position = scene.projectionView * worldPosition;
}