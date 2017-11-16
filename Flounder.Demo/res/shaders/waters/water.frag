#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 diffuseColour;
} object;

layout(binding = 3) uniform sampler2D samplerReflection;

layout(location = 0) flat in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) in vec4 fragmentClipSpace;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormal;

const float DUDV_STRENGTH = 0.02;

void main(void) 
{
	vec2 ndc = (fragmentClipSpace.xy / fragmentClipSpace.w) / 2.0 + 0.5;
	vec2 reflectTextures = vec2(ndc.x, -ndc.y);
	// vec2 refractTextures = vec2(ndc.x, ndc.y);
	
	vec4 reflectColour = texture(samplerReflection, reflectTextures);
	// vec4 refractColour = texture(samplerRefraction, refractTextures);
	
	// outColour = mix(reflectColour, refractColour, refractiveFactor);
	outColour = reflectColour;

	outColour = mix(outColour, object.diffuseColour, 0.5);
	outColour.a = 0.5f;
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}