#version 450

#extension GL_ARB_separate_shader_objects : enable

const int LIGHTS = 64;

layout(binding = 0) uniform sampler2D samplerAlbedo;
layout(binding = 1) uniform sampler2D samplerNormals;
layout(binding = 2) uniform sampler2D samplerExtras;
layout(binding = 3) uniform sampler2D samplerDepth;
layout(binding = 4) uniform sampler2D samplerShadows;

layout(binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;

	bool lightActive[LIGHTS];
	vec3 lightColour[LIGHTS];
	vec3 lightPosition[LIGHTS];
	vec3 lightAttenuation[LIGHTS];

	mat4 shadowSpaceMatrix;
	float shadowDistance;
	float shadowTransition;
	int shadowMapSize;
	int shadowPCF;
	float shadowBias;
	float shadowDarkness;

	float brightnessBoost;

	vec3 fogColour;
	float fogDensity;
	float fogGradient;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

vec3 decodeLocation() 
{
	float depth = texture(samplerDepth, textureCoords).x;
	vec4 p = inverse(ubo.projectionMatrix) * (vec4(textureCoords, depth, 1.0) * 2.0 - 1.0);
	return vec3(inverse(ubo.viewMatrix) * vec4(p.xyz / p.w, 1.0));
}

float shadow(sampler2D samplerShadows, vec4 shadowCoords, float shadowMapSize) 
{
	float totalTextels = (ubo.shadowPCF * 2.0 + 1.0) * (ubo.shadowPCF * 2.0 + 1.0);
	float texelSize = 1.0 / shadowMapSize;
	float total = 0.0;

	if (shadowCoords.x > 0.0 && shadowCoords.x < 1.0 && shadowCoords.y > 0.0 && shadowCoords.y < 1.0 && shadowCoords.z > 0.0 && shadowCoords.z < 1.0) 
	{
		for (int x = -ubo.shadowPCF; x <= ubo.shadowPCF; x++) 
		{
			for (int y = -ubo.shadowPCF; y <= ubo.shadowPCF; y++) 
			{
				float shadowValue = texture(samplerShadows, shadowCoords.xy + vec2(x, y) * texelSize).r;

				if (shadowCoords.z > shadowValue + ubo.shadowBias) 
				{
					total += ubo.shadowDarkness * shadowCoords.w;
				}
			}
		}

		total /= totalTextels;
	} 
	else
	{
		total = 0.0;
	}

	return 1.0 - total;
}

float visibility(vec4 positionRelativeToCam, float fogDensity, float fogGradient) 
{
	return clamp(exp(-pow((length(positionRelativeToCam.xyz) * ubo.fogDensity), ubo.fogGradient)), 0.0, 1.0);
}

void main(void) 
{
	// Reads all of the data passed to this fragment.
	vec4 albedo = texture(samplerAlbedo, textureCoords);
	vec3 normal = texture(samplerNormals, textureCoords).rgb * 2.0 - 1.0;
	vec4 extras = texture(samplerExtras, textureCoords);

	// Ignores anything this is not a rendered object, so mostly the cleared colour.
	if (albedo.a == 0.0) 
	{
		outColour = vec4(ubo.fogColour, 1.0);
		return;
	}

	// Sets a starting colour for this fragment.
	outColour = vec4(albedo.rgb, 1.0);

	// Gets the data from the extras texture.
	float shineDamper = extras.r;
	float glow = extras.g;
	bool ignoreFog = extras.b == (1.0 / 3.0) || extras.b == (3.0 / 3.0);
	bool ignoreLighting = extras.b == (2.0 / 3.0) || extras.b == (3.0 / 3.0);

	// Calculates some fragment data.
	vec4 worldPosition = vec4(decodeLocation(), 1.0);
	vec3 toCameraVector = (inverse(ubo.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	vec4 positionRelativeToCam = ubo.viewMatrix * worldPosition;

	if (!ignoreLighting) 
	{
		// Shadow mapping.
		if (ubo.shadowDarkness >= 0.07) 
		{
			vec4 shadowCoords = ubo.shadowSpaceMatrix * worldPosition;
			/*float distanceAway = length(positionRelativeToCam.xyz);
			distanceAway = distanceAway - ((ubo.shadowDistance * 2.0) - ubo.shadowTransition);
			distanceAway = distanceAway / ubo.shadowTransition;
			shadowCoords.w = clamp(1.0 - distanceAway, 0.0, 1.0);*/

			outColour *= shadow(samplerShadows, shadowCoords, ubo.shadowMapSize);
		}

		// Surface lighting.
		vec3 boost = vec3(ubo.brightnessBoost, ubo.brightnessBoost, ubo.brightnessBoost);
		vec3 totalDiffuse = vec3(0.0);
		vec3 totalSpecular = vec3(0.0);

		for (int i = 0; i < LIGHTS; i++) 
		{
			if (ubo.lightActive[i]) 
			{
				vec3 toLightVector = ubo.lightPosition[i] - worldPosition.xyz;
				vec3 unitLightVector = normalize(toLightVector);
				float distance = length(toLightVector);

				float attinuationFactor = ubo.lightAttenuation[i].x + (ubo.lightAttenuation[i].y * distance) + (ubo.lightAttenuation[i].z * distance * distance);

				float brightness = max(dot(normal, unitLightVector), 0.0);
				totalDiffuse = totalDiffuse + (brightness * ubo.lightColour[i]) / attinuationFactor;

			 //   vec3 reflectedLightDirection = reflect(-unitLightVector, normal);
			 //   float specularFactor = max(dot(reflectedLightDirection, normalize(toCameraVector)), 0.0);
			 //   float dampedFactor = pow(specularFactor, shineDamper);
			 //   totalSpecular = totalSpecular + (dampedFactor * glow * ubo.lightColour[i]) / attinuationFactor;
			}
		}

		outColour = (vec4(max(totalDiffuse, boost), 1.0) * outColour) + vec4(totalSpecular, 0.0);
	}

	if (!ignoreFog) 
	{
		outColour = mix(vec4(ubo.fogColour, 1.0), outColour, visibility(positionRelativeToCam, ubo.fogDensity, ubo.fogGradient));
	}
}
