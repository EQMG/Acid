#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

    vec4 textureOffsets;
    float blendFactor;
    float transparency;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerDiffuse;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

vec4 encodeColour(vec3 colour)
{
	vec4 result = vec4(0.0f);
	result.rgb = colour;
	result.a = 1.0f;
	return result;
}

void main() 
{
	vec3 sampledColour = texture(samplerDiffuse, fragmentUv).rgb;

	outColour = encodeColour(sampledColour);
	outNormal = vec2(0.0f);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
