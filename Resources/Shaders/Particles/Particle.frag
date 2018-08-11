#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 1) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec2 inUv;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() 
{
	vec3 albedo = texture(samplerAlbedo, inUv).rgb;

    outPosition = vec4(inWorldPos, 0.0f);
	outAlbedo = vec4(albedo, 1.0f);
	outNormal = vec4(0.0f);
	outMaterial = vec4(0.0f);
}
