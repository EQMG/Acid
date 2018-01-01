#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject 
{
	mat4 transform;
	vec4 skyColour;
	vec4 fogColour;
	vec2 fogLimits;
	float blendFactor;
} object;

layout(set = 0, binding = 2) uniform samplerCube samplerCubemap;

layout(location = 0) in vec3 fragmentUv;
layout(location = 1) in float fragmentHeight;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec3 outMaterial;

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
	result.x = (atan(normal.y, normal.x) / 3.14159f) * 0.5f + 0.5f;
	result.y = normal.z * 0.5f + 0.5f;
	return result;
}

void main() 
{
	vec3 cubemapColour = vec3(0.0f);
	
	if (object.blendFactor >= 0.03f)
	{
		vec3 cubemapNight = texture(samplerCubemap, fragmentUv).rgb;
		cubemapColour = mix(vec3(0.0f), cubemapNight, object.blendFactor);
	}

	cubemapColour += object.skyColour.rgb;

	float fadeFactor = 1.0 - smoothstep(object.fogLimits.x, object.fogLimits.y, fragmentHeight);
    cubemapColour = mix(cubemapColour, object.fogColour.rgb, fadeFactor);

	outColour = encodeColour(cubemapColour);
	outNormal = vec2(0.0f);
	outMaterial = vec3(0.0f);
}