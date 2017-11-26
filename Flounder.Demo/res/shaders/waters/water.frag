#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 diffuseColour;
} object;

layout(binding = 3) uniform sampler2D samplerReflection;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentUv;
layout(location = 2) in vec4 fragmentClipSpace;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec3 outExtras;

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

void main(void) 
{
	// vec2 ndc = (fragmentClipSpace.xy / fragmentClipSpace.w) / 2.0f + 0.5f;
	// vec2 reflectTextures = vec2(ndc.x, -ndc.y);
	// vec2 refractTextures = vec2(ndc.x, ndc.y);
	
	// vec3 textureReflection = texture(samplerReflection, reflectTextures).rgb;
	// vec3 textureRefraction = texture(samplerRefraction, refractTextures).rgb;
	
	// vec3 colour = mix(textureReflection, textureRefraction, refractiveFactor);
	// vec3 waterColour = textureReflection;
	// waterColour = mix(waterColour, object.diffuseColour.rgb, 0.5f);
	vec3 waterColour = object.diffuseColour.rgb;
	
	vec3 unitNormal = normalize(fragmentNormal);
	
	outColour = encodeColour(waterColour);
	outNormal = encodeNormal(unitNormal);
	outExtras = vec3(0.0f);
}