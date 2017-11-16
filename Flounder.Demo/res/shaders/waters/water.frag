#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 diffuseColour;
	vec3 cameraPosition;
	float moveFactor;
} object;

layout(binding = 3) uniform sampler2D samplerReflection;
layout(binding = 2) uniform sampler2D samplerDudv;

layout(location = 0) flat in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) in vec4 fragmentClipSpace;
layout(location = 3) in vec3 fragmentCameraVector;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec4 outNormal;

const float DUDV_STRENGTH = 0.02;

void main(void) 
{
	vec2 ndc = (fragmentClipSpace.xy / fragmentClipSpace.w) / 2.0 + 0.5;
	vec2 reflectTextures = vec2(ndc.x, -ndc.y);
	// vec2 refractTextures = vec2(ndc.x, ndc.y);
	
	vec2 distortion1 = DUDV_STRENGTH * (texture(samplerDudv, 50.0 * vec2(fragmentTextures.x + object.moveFactor, fragmentTextures.y)).rg * 2.0 - 1.0);
	vec2 distortion2 = DUDV_STRENGTH * (texture(samplerDudv, 50.0 * vec2(-fragmentTextures.x, fragmentTextures.y + object.moveFactor)).rg * 2.0 - 1.0);
	vec2 distortionTotal = distortion1 + distortion2;
	
	reflectTextures += distortionTotal;
	reflectTextures.x = clamp(reflectTextures.x, 0.001, 0.999);
	reflectTextures.y = clamp(reflectTextures.y, -0.999, -0.001);
	// refractTextures += distortionTotal;
	// refractTextures = clamp(refractTextures, 0.001, 0.999);
	
	vec4 reflectColour = texture(samplerReflection, reflectTextures);
	// vec4 refractColour = texture(samplerRefraction, refractTextures);
	vec3 viewVector = normalize(fragmentCameraVector);
	float refractiveFactor = dot(viewVector, fragmentNormal);
	refractiveFactor = pow(refractiveFactor, 0.5);
	refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
	
	// outColour = mix(reflectColour, refractColour, refractiveFactor);
	// outColour = reflectColour;
	outColour = mix(reflectColour, object.diffuseColour, refractiveFactor);

	// outColour = mix(outColour, object.diffuseColour, 0.5);
	outColour.a = 0.5f;
	outNormal = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
}