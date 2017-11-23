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
layout(location = 1) out vec3 outNormal;

vec4 encodeColour(vec3 colour)
{
	vec4 result = vec4(0.0f);
	result.rgb = colour;
	result.a = 1.0f;
	return result;
}

vec2 encodeNormal(vec3 normal)
{
	vec2 result = vec2(0.0f);
	result.x = atan(normal.y, normal.x) / 3.14159f;
	result.y = normal.z;
	return result * 0.5f + 0.5f;
}

void main(void) 
{
	vec3 cubemapColour = vec3(0.0f);
	
	if (object.blendFactor >= 0.03f)
	{
		vec3 cubemapNight = texture(samplerCubemap, fragmentTextures).rgb;
		cubemapColour = mix(vec3(0.0f), cubemapNight, object.blendFactor);
	}

	cubemapColour += object.skyColour.rgb;

	outColour = encodeColour(cubemapColour);
	outNormal = vec3(0.0f);
}