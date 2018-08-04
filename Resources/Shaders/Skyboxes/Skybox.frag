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
layout(location = 2) out vec4 outMaterial;

#include "Shaders/Pipeline.glsl"

void main() 
{
	vec3 cubemapNight = texture(samplerCubemap, fragmentUv).rgb;
	vec3 cubemapColour = mix(object.skyColour.rgb, cubemapNight, object.blendFactor);

	float fadeFactor = 1.0f - smoothstep(object.fogLimits.x, object.fogLimits.y, fragmentHeight);
    cubemapColour = mix(cubemapColour, object.fogColour.rgb, fadeFactor);

	outColour = vec4(cubemapColour, 1.0f);
	outNormal = vec2(0.0f);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}