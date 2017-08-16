#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UBO 
{
	vec4 colour;
	vec4 borderColour;
	vec2 borderSizes;
	vec2 edgeData;
	
	float aspectRatio;
	vec2 size;
	vec4 transform;
	float rotation;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTextureCoords;

layout(location = 0) out vec2 textureCoords;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main(void) 
{
	vec2 screenPosition = vec2(
		(inPosition.x - ubo.size.x) * ubo.transform.z * cos(ubo.rotation) - 
		(inPosition.y - ubo.size.y) * ubo.transform.w * sin(ubo.rotation),
		(inPosition.x - ubo.size.x) * ubo.transform.z * sin(ubo.rotation) + 
		(inPosition.y - ubo.size.y) * ubo.transform.w * cos(ubo.rotation)
	);

	screenPosition = screenPosition + ubo.transform.xy;
	screenPosition.x = (screenPosition.x / ubo.aspectRatio) * 2.0 - 1.0;
	screenPosition.y = screenPosition.y * -2.0 + 1.0;
	gl_Position = vec4(screenPosition, 0.0, 1.0);

	textureCoords = inTextureCoords;
}
