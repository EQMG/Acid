#version

//---------INCLUDES------------
#include "maths.glsl"

//---------CONSTANT------------
const int LIGHTS = 64;

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalAlbedo;
layout(binding = 1) uniform sampler2D originalNormals;
layout(binding = 2) uniform sampler2D originalExtras;
layout(binding = 3) uniform sampler2D originalDepth;
layout(binding = 4) uniform sampler2D shadowMap;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform bool lightActive[LIGHTS];
uniform vec3 lightColour[LIGHTS];
uniform vec3 lightPosition[LIGHTS];
uniform vec3 lightAttenuation[LIGHTS];

uniform mat4 shadowSpaceMatrix;
uniform float shadowDistance;
uniform float shadowTransition;
uniform int shadowMapSize;
uniform int shadowPCF;
uniform float shadowBias;
uniform float shadowDarkness;

uniform float brightnessBoost;

uniform vec3 fogColour;
uniform float fogDensity;
uniform float fogGradient;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------CALCULATE LOCATION------------
vec3 decodeLocation() {
    float depth = texture(originalDepth, pass_textureCoords).x;
    vec4 p = finverse(projectionMatrix) * (vec4(pass_textureCoords, depth, 1.0) * 2.0 - 1.0);
    return vec3(finverse(viewMatrix) * vec4(p.xyz / p.w, 1.0));
}

//---------SHADOW------------
float shadow(sampler2D shadowMap, vec4 shadowCoords, float shadowMapSize) {
    float totalTextels = (shadowPCF * 2.0 + 1.0) * (shadowPCF * 2.0 + 1.0);
    float texelSize = 1.0 / shadowMapSize;
    float total = 0.0;

    if (shadowCoords.x > 0.0 && shadowCoords.x < 1.0 && shadowCoords.y > 0.0 && shadowCoords.y < 1.0 && shadowCoords.z > 0.0 && shadowCoords.z < 1.0) {
        for (int x = -shadowPCF; x <= shadowPCF; x++) {
            for (int y = -shadowPCF; y <= shadowPCF; y++) {
                float shadowValue = texture(shadowMap, shadowCoords.xy + vec2(x, y) * texelSize).r;

                if (shadowCoords.z > shadowValue + shadowBias) {
                    total += shadowDarkness * shadowCoords.w;
                }
            }
        }

        total /= totalTextels;
    } else {
        total = 0.0;
    }

    return 1.0 - total;
}

//---------FOG VISIBILITY------------
float visibility(vec4 positionRelativeToCam, float fogDensity, float fogGradient) {
	return clamp(exp(-pow((length(positionRelativeToCam.xyz) * fogDensity), fogGradient)), 0.0, 1.0);
}

//---------MAIN------------
void main(void) {
    // Reads all of the data passed to this fragment.
	vec4 albedo = texture(originalAlbedo, pass_textureCoords);
	vec3 normal = texture(originalNormals, pass_textureCoords).rgb * 2.0 - 1.0;
	vec4 extras = texture(originalExtras, pass_textureCoords);

	// Ignores anything this is not a rendered object, so mostly the cleared colour.
	if (albedo.a == 0.0) {
	    out_colour = vec4(fogColour, 1.0);
	    return;
	}

    // Sets a starting colour for this fragment.
    out_colour = vec4(albedo.rgb, 1.0);

	// Gets the data from the extras texture.
	float shineDamper = extras.r;
	float glow = extras.g;
	bool ignoreFog = extras.b == (1.0 / 3.0) || extras.b == (3.0 / 3.0);
	bool ignoreLighting = extras.b == (2.0 / 3.0) || extras.b == (3.0 / 3.0);

    // Calculates some fragment data.
	vec4 worldPosition = vec4(decodeLocation(), 1.0);
	vec3 toCameraVector = (finverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    vec4 positionRelativeToCam = viewMatrix * worldPosition;

    if (!ignoreLighting) {
        // Shadow mapping.
        if (shadowDarkness >= 0.07) {
            vec4 shadowCoords = shadowSpaceMatrix * worldPosition;
            float distanceAway = length(positionRelativeToCam.xyz);
            distanceAway = distanceAway - ((shadowDistance * 2.0) - shadowTransition);
            distanceAway = distanceAway / shadowTransition;
            shadowCoords.w = clamp(1.0 - distanceAway, 0.0, 1.0);

            out_colour *= shadow(shadowMap, shadowCoords, shadowMapSize);
        }

        // Surface lighting.
        vec3 boost = vec3(brightnessBoost, brightnessBoost, brightnessBoost);
        vec3 totalDiffuse = vec3(0.0);
        vec3 totalSpecular = vec3(0.0);

        for (int i = 0; i < LIGHTS; i++) {
            if (lightActive[i]) {
                vec3 toLightVector = lightPosition[i] - worldPosition.xyz;
                vec3 unitLightVector = normalize(toLightVector);
                float distance = length(toLightVector);

                float attinuationFactor = lightAttenuation[i].x + (lightAttenuation[i].y * distance) + (lightAttenuation[i].z * distance * distance);

                float brightness = max(dot(normal, unitLightVector), 0.0);
                totalDiffuse = totalDiffuse + (brightness * lightColour[i]) / attinuationFactor;

             //   vec3 reflectedLightDirection = reflect(-unitLightVector, normal);
             //   float specularFactor = max(dot(reflectedLightDirection, normalize(toCameraVector)), 0.0);
             //   float dampedFactor = pow(specularFactor, shineDamper);
             //   totalSpecular = totalSpecular + (dampedFactor * glow * lightColour[i]) / attinuationFactor;
            }
        }

        out_colour = (vec4(max(totalDiffuse, boost), 1.0) * out_colour) + vec4(totalSpecular, 0.0);

    }

    if (!ignoreFog) {
        out_colour = mix(vec4(fogColour, 1.0), out_colour, visibility(positionRelativeToCam, fogDensity, fogGradient));
    }
}
