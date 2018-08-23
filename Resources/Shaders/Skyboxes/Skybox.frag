#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform UboObject 
{
	mat4 transform;
	vec4 skyColour;
	vec4 fogColour;
	vec2 fogLimits;
	float blendFactor;
} object;

layout(set = 0, binding = 2) uniform samplerCube samplerCubemap;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inUv;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() 
{
	vec3 nightColour = texture(samplerCubemap, inUv).rgb;
	vec3 colour = mix(object.skyColour.rgb, nightColour, object.blendFactor);

	float fadeFactor = 1.0f - smoothstep(object.fogLimits.x, object.fogLimits.y, inWorldPos.y);
	colour = mix(colour, object.fogColour.rgb, fadeFactor);

	outPosition = vec4(inWorldPos, 1.0);
	outDiffuse = vec4(colour, 1.0f);
	outNormal = vec4(0.0f);
	outMaterial = vec4(0.0f);
}