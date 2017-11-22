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

vec3 decodeColour(vec4 colour)
{
	//vec2 rg = unpackHalf2x16(colour.r);
	//vec2 bd = unpackHalf2x16(colour.b);

	vec3 result = vec3(0.0f);
	result.r = colour.r; // rg.x;
	result.g = colour.g; // rg.y;
	result.b = colour.b; // bd.x;
    return result;
}

float decodeDepth(vec4 colour)
{
	//vec2 bd = unpackHalf2x16(colour.b);

	float result = 0.0f;
	result = 0.0f;// bd.y;
	return result;
}

vec3 decodeNormal(vec4 normal)
{
	vec2 spherical = normal.xy * 2.0f - 1.0f;
	vec2 sinCosTheta = vec2(sin(spherical.x * 3.14159), cos(spherical.x * 3.14159));
	vec2 sinCosPhi = vec2(sqrt(1.0f - spherical.y * spherical.y), spherical.y);
	return vec3(sinCosTheta.y * sinCosPhi.x, sinCosTheta.x * sinCosPhi.x, sinCosPhi.y);
}

vec3 decodePosition(float depth) 
{
    vec4 p = inverse(scene.projection) * (vec4(fragmentTextures, depth, 1.0) * 2.0 - 1.0);
    return vec3(inverse(scene.view) * vec4(p.xyz / p.w, 1.0));
}

float shadow(sampler2D shadowMap, vec4 shadowCoords, float shadowMapSize) 
{
    float totalTextels = (scene.shadow.shadowPCF * 2.0f + 1.0f) * (scene.shadow.shadowPCF * 2.0f + 1.0f);
    float texelSize = 1.0f / scene.shadow.shadowMapSize;
    float total = 0.0f;

    if (shadowCoords.x > 0.0f && shadowCoords.x < 1.0f && shadowCoords.y > 0.0f && shadowCoords.y < 1.0f && shadowCoords.z > 0.0f && shadowCoords.z < 1.0f) 
	{
        for (int x = -scene.shadow.shadowPCF; x <= scene.shadow.shadowPCF; x++) 
		{
            for (int y = -scene.shadow.shadowPCF; y <= scene.shadow.shadowPCF; y++) 
			{
                float shadowValue = texture(samplerShadows, shadowCoords.xy + vec2(x, y) * texelSize).r;

                if (shadowCoords.z > shadowValue + scene.shadow.shadowBias) 
				{
                    total += scene.shadow.shadowDarkness * shadowCoords.w;
                }
            }
        }

        total /= totalTextels;
    } 
	else 
	{
        total = 0.0f;
    }

    return 1.0f - total;
}

void main(void) 
{
	vec4 textureColour = texture(samplerColour, fragmentTextures);
	vec4 textureNormal = texture(samplerNormal, fragmentTextures);
	
	vec3 colour = decodeColour(textureColour);
	float depth = decodeDepth(textureColour);
	vec3 normal = decodeNormal(textureNormal);
	vec3 position = decodePosition(depth);
	
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for (int i = 0; i < NUMBER_LIGHTS; i++) 
	{
        if (scene.lights[i].enabled == 1.0f) 
		{
            vec3 toLightVector = scene.lights[i].position - position.xyz;
            vec3 unitLightVector = normalize(toLightVector);
            float lightDistance = length(toLightVector);

            float attinuationFactor = scene.lights[i].attenuation.x + (scene.lights[i].attenuation.y * lightDistance) + (scene.lights[i].attenuation.z * lightDistance * lightDistance);

            float lightBrightness = max(dot(normal, unitLightVector), 0.0f);
            totalDiffuse = totalDiffuse + (lightBrightness * scene.lights[i].colour.rgb) / attinuationFactor;

         //   vec3 reflectedLightDirection = reflect(-unitLightVector, normal);
         //   float specularFactor = max(dot(reflectedLightDirection, normalize(toCameraVector)), 0.0f);
         //   float dampedFactor = pow(specularFactor, shineDamper);
         //   totalSpecular = totalSpecular + (dampedFactor * glow * scene.lights[i].colour) / attinuationFactor;
        }
    }

	outColour = vec4((totalDiffuse * colour) + totalSpecular, 1.0f);

	outColour = vec4(normal, 1.0f);
}
