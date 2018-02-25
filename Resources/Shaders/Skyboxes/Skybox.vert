#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject 
{
	mat4 transform;
	vec4 skyColour;
	vec4 fogColour;
	vec2 fogLimits;
	float blendFactor;
} object;

layout(location = 0) in vec3 vertexPosition;

layout(location = 0) out vec3 fragmentUv;
layout(location = 1) out float fragmentHeight;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(vertexPosition, 1.0f);
	
    gl_Position = scene.projection * scene.view * worldPosition;
	
	fragmentUv = vertexPosition;
	fragmentHeight = worldPosition.y;
}
