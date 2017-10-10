#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform samplerCube samplerCubemap;

layout(binding = 1) uniform UboObject 
{
	mat4 transform;
	vec3 skyColour;
	float blendFactor;
} object;

layout(location = 0) in vec3 fragmentTextures;

layout(location = 0) out vec4 outColour;
//layout(location = 1) out vec4 outNormals;
//layout(location = 2) out vec4 outExtras;

void main(void) 
{
	vec3 cubemapColour = texture(samplerCubemap, fragmentTextures).rgb; // vec3(object.skyColour);

	outColour = vec4(cubemapColour, 1.0); // cubemapColour // object.skyColour + mix(vec3(0.0), cubemapColour, object.blendFactor)
//	outNormals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 0.0);
//	outExtras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}