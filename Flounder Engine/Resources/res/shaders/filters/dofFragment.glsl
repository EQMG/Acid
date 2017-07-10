#version

//---------INCLUDES------------
#include "maths.glsl"

//---------CONSTANT------------
const float NEAR_FIELD = 0.3;
const float NEAR_TRANS = 0.125;
const float NEAR_END = NEAR_FIELD + NEAR_TRANS;

const float FAR_FIELD = 8.0;
const float FAR_TRANS = 3.0;
const float FAR_START = FAR_FIELD - FAR_TRANS;

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
layout(binding = 1) uniform sampler2D depthTexture;
layout(binding = 2) uniform sampler2D blurredTexture;
uniform float aimDistance;
uniform float nearPlane;
uniform float farPlane;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------GET DEPTH FACTOR------------
float getDepthFactor(float depth, float upperLimit){
	return clamp(depth/upperLimit, 0.0, 1.0);
}

//---------CALCULATE DEPTH------------
float calculateDepth(){
	float depth = texture(depthTexture, pass_textureCoords).r;
	return 2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));
}

//---------MAIN------------
void main(void) {
	vec3 originalColour = texture(originalTexture, pass_textureCoords).rgb;
	vec3 blurColour = texture(blurredTexture, pass_textureCoords).rgb;

	float depth = calculateDepth();
	float nearVisibility = fsmoothlyStep(NEAR_FIELD * aimDistance, NEAR_END * aimDistance , depth);
	float farVisibility = 1.0 - fsmoothlyStep(FAR_START * aimDistance, FAR_FIELD * aimDistance, depth);
	vec3 totalColour = mix(blurColour, originalColour, nearVisibility);
	totalColour = mix(blurColour, totalColour, farVisibility);
	out_colour = vec4(totalColour, 1.0);
}
