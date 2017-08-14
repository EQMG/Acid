#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform samplerCube samplerTexture;

layout(binding = 1) uniform UBO 
{
	vec3 skyColour;
	float blendFactor;
} ubo;

layout(location = 0) in vec3 textureCoords;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormals;
layout(location = 2) out vec4 outExtras;

void main(void) 
{
	vec3 cubemapColour = texture(samplerTexture, textureCoords).rgb;

	outAlbedo = vec4(ubo.skyColour + mix(vec3(0.0), cubemapColour, ubo.blendFactor), 1.0);
	outNormals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 0.0);
	outExtras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}