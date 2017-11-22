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
layout(location = 1) out vec2 outNormal;

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
	vec2 ndc = (fragmentClipSpace.xy / fragmentClipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTextures = vec2(ndc.x, -ndc.y);
	// vec2 refractTextures = vec2(ndc.x, ndc.y);
	
	vec3 textureReflection = texture(samplerReflection, reflectTextures).rgb;
	// vec3 textureRefraction = texture(samplerRefraction, refractTextures).rgb;
	
	vec3 unitNormal = normalize(fragmentNormal);
	
	// vec3 colour = mix(textureReflection, textureRefraction, refractiveFactor);
	vec3 waterColour = textureReflection;
	waterColour = mix(waterColour, object.diffuseColour.rgb, 0.5f);
	
	outColour = encodeColour(waterColour);
	outNormal = encodeNormal(unitNormal);
}