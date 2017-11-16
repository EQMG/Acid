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

layout(binding = 1) uniform sampler2D samplerColour;
layout(binding = 2) uniform sampler2D samplerNormal;
layout(binding = 3) uniform sampler2D samplerShadows;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

const vec2 lightBias = vec2(0.3, 0.8);
const vec3 lightColour = vec3(1.0, 0.95, 0.95);
const vec3 lightDirection = vec3(0.2, -0.3, 0.2);

void main(void) 
{
	vec4 colour = texture(samplerColour, fragmentTextures);
	vec4 normal = texture(samplerNormal, fragmentTextures);
	
	vec3 lighting = vec3(1.0);
	
	if (normal.a != 0.0)
	{
		vec3 unitNormal = normal.rgb * 2.0 - 1.0;
		float brightness = max(dot(-lightDirection, unitNormal), 0.0);
		lighting = (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
	}

	outColour = vec4(colour.rgb * lighting, 1.0);

//	float grey = dot(colour.rgb, vec3(0.299, 0.587, 0.114));
//	outColour = vec4(grey, grey, grey, 1.0);
}
