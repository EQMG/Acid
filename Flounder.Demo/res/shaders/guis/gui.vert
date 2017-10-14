#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UboScene
{
	float aspectRatio;
} scene;

layout(binding = 1) uniform UboObject
{
	vec4 scissor;
	vec2 size;
	vec4 transform;
	float rotation;
	float alpha;
	float flipTexture;
	float atlasRows;
	vec2 atlasOffset;
	vec4 colourOffset;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextures;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(location = 0) out vec2 fragmentTextures;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main(void)
{
	vec2 screenPosition = vec2(
		(vertexPosition.x - object.size.x) * object.transform.z * cos(object.rotation) - 
		(vertexPosition.y - object.size.y) * object.transform.w * sin(object.rotation),
		(vertexPosition.x - object.size.x) * object.transform.z * sin(object.rotation) + 
		(vertexPosition.y - object.size.y) * object.transform.w * cos(object.rotation)
	);
	screenPosition = screenPosition + object.transform.xy;
	screenPosition.x = (screenPosition.x / scene.aspectRatio) * 2.0 - 1.0;
	screenPosition.y = screenPosition.y * -2.0 + 1.0;
	gl_Position = vec4(screenPosition, 0.0, 1.0);

	fragmentTextures = vertexPosition.xy;
	fragmentTextures = (fragmentTextures / object.atlasRows) + object.atlasOffset;
	fragmentTextures.x = mix(fragmentTextures.x, 1.0 - fragmentTextures.x, object.flipTexture);
}
