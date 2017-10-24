#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject 
{
	mat4 transform;
	vec4 skyColour;
	float blendFactor;
} object;

layout(binding = 2) uniform samplerCube samplerCubemap;

layout(location = 0) in vec3 fragmentTextures;

layout(location = 0) out vec4 outColour;
//layout(location = 1) out vec4 outNormals;
//layout(location = 2) out vec4 outExtras;

void main(void) 
{
	vec3 cubemapColour = vec3(0.0);
	
	if (object.blendFactor >= 0.03)
	{
		vec3 cubemapNight = texture(samplerCubemap, fragmentTextures).rgb;
		cubemapColour = mix(vec3(0.0), cubemapNight, object.blendFactor);
	}

	outColour = vec4(object.skyColour.rgb + cubemapColour, 1.0);
//	outNormals = vec4(vec3(0.0, 1.0, 0.0) + 1.0 / 2.0, 0.0);
//	outExtras = vec4(1.0, 0.0, 1.0, 1.0); // Ignores lighting.
}