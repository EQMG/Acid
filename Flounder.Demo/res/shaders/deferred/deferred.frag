#version 450

#extension GL_ARB_separate_shader_objects : enable

#define NUMBER_LIGHTS 32

layout(binding = 0) uniform UboScene 
{
	mat4 projection;
	mat4 view;
	mat4 shadowSpace;

	vec4 fogColour;
	float fogDensity;
	float fogGradient;

	float shadowDistance;
	float shadowTransition;
	float shadowBias;
	float shadowDarkness;
	int shadowMapSize;
	int shadowPCF;
} scene;

layout(binding = 1) uniform Ubolights 
{
	vec4 lightColours[NUMBER_LIGHTS];
	vec3 lightPositions[NUMBER_LIGHTS];
	vec3 lightAttenuations[NUMBER_LIGHTS];
} lights;

layout(binding = 2) uniform sampler2D samplerColour;
layout(binding = 3) uniform sampler2D samplerNormal;
layout(binding = 4) uniform sampler2D samplerExtras;
layout(binding = 5) uniform sampler2D samplerShadows;

layout(location = 0) in vec2 fragmentTextures;

layout(location = 0) out vec4 outColour;

vec3 decodeColour(vec4 colour)
{
	vec3 result = vec3(0.0f);
	result.r = colour.r; // rg.x;
	result.g = colour.g; // rg.y;
	result.b = colour.b; // bd.x;
    return result;
}

vec3 decodeNormal(vec4 normal)
{
	vec2 spherical = normal.xy * 2.0f - 1.0f;
	vec2 sinCosTheta = vec2(sin(spherical.x * 3.14159f), cos(spherical.x * 3.14159f));
	vec2 sinCosPhi = vec2(sqrt(1.0f - spherical.y * spherical.y), spherical.y);
	return vec3(sinCosTheta.y * sinCosPhi.x, sinCosTheta.x * sinCosPhi.x, sinCosPhi.y);
}

float decodeDepth(vec4 normal)
{
	return normal.z;
}

vec3 decodePosition(float depth) 
{
	vec4 p = inverse(scene.projection) * (vec4(fragmentTextures, depth, 1.0f) * 2.0f - 1.0f);
	return vec3(inverse(scene.view) * vec4(p.xyz / p.w, 1.0f));
}

float shadow(sampler2D shadowMap, vec4 shadowCoords, float shadowMapSize) 
{
    float totalTextels = (scene.shadowPCF * 2.0f + 1.0f) * (scene.shadowPCF * 2.0f + 1.0f);
    float texelSize = 1.0f / scene.shadowMapSize;
    float total = 0.0f;

    if (shadowCoords.x > 0.0f && shadowCoords.x < 1.0f && shadowCoords.y > 0.0f && shadowCoords.y < 1.0f && shadowCoords.z > 0.0f && shadowCoords.z < 1.0f) 
	{
        for (int x = -scene.shadowPCF; x <= scene.shadowPCF; x++) 
		{
            for (int y = -scene.shadowPCF; y <= scene.shadowPCF; y++) 
			{
                float shadowValue = texture(samplerShadows, shadowCoords.xy + vec2(x, y) * texelSize).r;

                if (shadowCoords.z > shadowValue + scene.shadowBias) 
				{
                    total += scene.shadowDarkness * shadowCoords.w;
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
	vec4 textureExtras = texture(samplerExtras, fragmentTextures);

	vec3 colour = decodeColour(textureColour);
	vec3 normal = decodeNormal(textureNormal);
	float depth = decodeDepth(textureNormal);
	vec3 position = decodePosition(depth);
	vec3 toCameraVector = (inverse(scene.view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - position;
	vec4 positionRelativeToCam = scene.view * vec4(position, 1.0f);
	
	outColour = vec4(colour, 1.0f);
	
	// Shadowing.
	if (textureNormal.rgb != vec3(0.0f))
	{
		// vec4 shadowCoords = scene.shadowSpace * vec4(position, 1.0f);
		// float distanceAway = length(positionRelativeToCam.xyz);
		// distanceAway = distanceAway - ((scene.shadowDistance * 2.0f) - scene.shadowTransition);
		// distanceAway = distanceAway / scene.shadowTransition;
		// shadowCoords.w = clamp(1.0f - distanceAway, 0.0f, 1.0f);
		// outColour *= shadow(samplerShadows, shadowCoords, scene.shadowMapSize);
	}

	// Lighting.
	if (textureNormal.rgb != vec3(0.0f))
	{
		vec3 totalDiffuse = vec3(0.0);
		vec3 totalSpecular = vec3(0.0);
		float shineDamper = 0.1f;

		for (int i = 0; i < NUMBER_LIGHTS; i++) 
		{
			if (lights.lightColours[i].a != 0.0f) 
			{
				vec3 toLightVector = lights.lightPositions[i] - position.xyz;
				vec3 unitLightVector = normalize(toLightVector);
				float lightDistance = length(toLightVector);
			
				float attinuationFactor = lights.lightAttenuations[i].x + (lights.lightAttenuations[i].y * lightDistance) + (lights.lightAttenuations[i].z * lightDistance * lightDistance);
			
				float lightBrightness = max(dot(normal, unitLightVector), 0.0f);
				totalDiffuse += (lightBrightness * lights.lightColours[i].rgb) / attinuationFactor;

			//  vec3 reflectedLightDirection = reflect(-unitLightVector, normal);
			//  float specularFactor = max(dot(reflectedLightDirection, normalize(toCameraVector)), 0.0f);
			//  float dampedFactor = pow(specularFactor, shineDamper);
			//  totalSpecular += (dampedFactor * lights.lightColours[i].rgb) / attinuationFactor;
			}
		}

		totalDiffuse = max(totalDiffuse, vec3(0.3f));
		outColour = vec4((totalDiffuse * outColour.rgb) + totalSpecular, 1.0f);
	}
	
	// Fog.
	if (textureNormal.rgb != vec3(0.0f))
	{
		// float fogFactor = exp(-pow(length(positionRelativeToCam.xyz) * scene.fogDensity, scene.fogGradient));
		// fogFactor = clamp(fogFactor, 0.0, 1.0);
		// outColour = mix(scene.fogColour, outColour, fogFactor);
	}
}
