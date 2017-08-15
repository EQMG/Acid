#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UBO 
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vec4 clipPlane;
	float numberOfRows;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in mat4 inModelMatrix;
layout(location = 5) in vec4 inTextureOffsets;
layout(location = 6) in float inBlendFactor;
layout(location = 7) in float inTransparency;

layout(location = 0) out vec2 textureCoords1;
layout(location = 1) out vec2 textureCoords2;
layout(location = 2) out float textureBlendFactor;
layout(location = 3) out float textureTransparency;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main(void) 
{
	mat4 modelViewMatrix = ubo.viewMatrix * inModelMatrix;
	gl_ClipDistance[0] = dot(inModelMatrix * vec4(inPosition, 0.0, 1.0), ubo.clipPlane);
	gl_Position = ubo.projectionMatrix * modelViewMatrix * vec4(inPosition, 0.0, 1.0);

	vec2 textureCoords = inPosition + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;
	textureCoords /= ubo.numberOfRows;

	textureCoords1 = textureCoords + inTextureOffsets.xy;
	textureCoords2 = textureCoords + inTextureOffsets.zw;
	textureBlendFactor = inBlendFactor;
	textureTransparency = inTransparency;
}
