#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UBO 
{
	float aspectRatio;
	vec2 size;
	vec4 transform;
	float rotation;
	bool flipTexture;
	float atlasRows;
	vec2 atlasOffset;
} ubo;

layout(location = 0) in vec2 inPosition;

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

	textureCoords = inPosition;
	textureCoords = (textureCoords / ubo.atlasRows) + ubo.atlasOffset;
	textureCoords.x = mix(textureCoords.x, 1.0 - textureCoords.x, ubo.flipTexture);
}
