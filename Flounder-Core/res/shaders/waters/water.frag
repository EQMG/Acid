#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D reflectionMap;

layout(binding = 1) uniform UBO 
{
	vec4 diffuseColour;

	float shineDamper;
	float reflectivity;

	bool ignoreReflections;
} ubo;

layout(location = 0) in vec3 surfaceNormal;
layout(location = 1) in vec4 clipSpace;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormals;
layout(location = 2) out vec4 outExtras;

//---------REFRACTION------------
vec2 getReflectionTexCoords(vec2 normalizedDeviceCoords)
{
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x, -normalizedDeviceCoords.y);
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	return reflectionTextureCoords;
}

//---------MAIN------------
void main(void) 
{
	if (!ubo.ignoreReflections) 
	{
		vec2 normalizedDeviceCoords = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
		vec2 reflectionTextureCoords = getReflectionTexCoords(normalizedDeviceCoords);
		vec3 reflectionColour = texture(reflectionMap, reflectionTextureCoords).rgb;
		outAlbedo = vec4(mix(reflectionColour, ubo.diffuseColour.rgb, ubo.diffuseColour.a), 1.0);
	} 
	else 
	{
		outAlbedo = vec4(ubo.diffuseColour.rgb, 1.0);
	}

	outNormals = vec4(surfaceNormal + 1.0 / 2.0, 1.0);
	outExtras = vec4(ubo.shineDamper, ubo.reflectivity, (1.0 / 3.0) * (float(false) + 2.0 * float(false)), 1.0);
}