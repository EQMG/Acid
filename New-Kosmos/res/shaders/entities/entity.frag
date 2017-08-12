#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerAlbedo;
layout(binding = 1) uniform sampler2D samplerGlow;

layout(binding = 0) uniform UBO 
{
	vec3 colourOffset;
	vec3 colourAddition;

	float transparency;

	float shineDamper;
	float reflectivity;

	bool ignoreFog;
	bool ignoreLighting;

	bool useGlowMap;
} ubo;

layout(location = 0) in vec2 textureCoords;
layout(location = 1) in vec3 surfaceNormal;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormals;
layout(location = 2) out vec4 outExtras;

void main(void) 
{
	vec4 diffuseColour = texture(samplerAlbedo, textureCoords);

	if (diffuseColour.a < 0.1)
	{
		outAlbedo = vec4(0.0);
		outNormals = vec4(0.0);
		outExtras = vec4(0.0);
		discard;
	}

	bool glowing = false;
	float glow = ubo.reflectivity;

	if (ubo.useGlowMap) 
	{
		vec4 glowColour = texture(samplerGlow, textureCoords);
		glow += length(glowColour.rgb);

		if (glowColour.r > 0.5) 
		{
			glowing = true;
		}
	}

	outAlbedo = vec4(diffuseColour + vec4(ubo.colourOffset, 0.0)) + vec4(ubo.colourAddition, 0.0);
	outAlbedo.a *= 1.0 - ubo.transparency;
	outNormals = vec4(surfaceNormal + 1.0 / 2.0, outAlbedo.a);
	outExtras = vec4(ubo.shineDamper, glow, (1.0 / 3.0) * (float(ubo.ignoreFog) + (2.0 * float(ubo.ignoreLighting || glowing))), outAlbedo.a);
}