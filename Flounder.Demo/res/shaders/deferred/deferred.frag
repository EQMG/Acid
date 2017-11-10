#version 450

#extension GL_ARB_separate_shader_objects : enable

struct Light
{
	vec4 colour;
	vec3 position;
	vec3 attenuation;
	float enabled;
};

struct Shadow
{
	mat4 shadowSpaceMatrix;
	float shadowDistance;
	float shadowTransition;
	float shadowBias;
	float shadowDarkness;
	int shadowMapSize;
	int shadowPCF;
};

struct Fog
{
	vec4 colour;
	float density;
	float gradient;
};

#define NUMBER_LIGHTS 32

layout(binding = 0) uniform UboScene 
{
	Light lights[NUMBER_LIGHTS];
	Shadow shadow;
	Fog fog;

	mat4 projection;
	mat4 view;

} scene;

// layout(binding = 1) uniform sampler2D samplerDepth;
layout(binding = 2) uniform sampler2D samplerColour;
layout(binding = 3) uniform sampler2D samplerNormal;
layout(binding = 4) uniform sampler2D samplerShadows;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	vec4 colour = texture(samplerColour, fragmentTextures);
	vec4 normal = texture(samplerNormal, fragmentTextures);
	float grey = dot(colour.rgb, vec3(0.299, 0.587, 0.114));
	
	// outColour = vec4(grey, grey, grey, 1.0);
	outColour = vec4(colour.rgb, 1.0);
}
