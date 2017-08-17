#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
	vec4 clipPlane;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 textureCoords;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main(void) 
{
	vec4 worldPosition = ubo.modelMatrix * vec4(inPosition, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, ubo.clipPlane);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;

	textureCoords = inPosition;
}
