#version 450
#extension GL_ARB_separate_shader_objects : enable

const float SAMPLES = 8.0;

layout(set = 0, binding = 0) uniform sampler2D samplerAlbedo;
layout(set = 0, binding = 1) uniform sampler2D samplerDepth;

layout(set = 0, binding = 2) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 lastViewMatrix;
	float delta;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

vec3 decodeLocation() 
{
	float depth = texture(samplerDepth, fragmentUv).r;
	vec4 p = inverse(ubo.projectionMatrix) * (vec4(fragmentUv, depth, 1.0) * 2.0 - 1.0);
	return vec3(inverse(ubo.viewMatrix) * vec4(p.xyz / p.w, 1.0));
}

void main() 
{
	// Gets the currebt world position for this fragment.
	vec4 worldPosition = vec4(decodeLocation(), 1.0);

	// Transform by the view-projection inverse to get the current world position.
	vec4 currentPos = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;

	// Use the world position, and transform by the previous view-projection matrix.
	vec4 previousPos = ubo.projectionMatrix * ubo.lastViewMatrix * worldPosition;

	// Use this frame's position and last frame's to compute the pixel velocity.
	vec2 velocity = (currentPos.xy - previousPos.xy) * 0.02 * ubo.delta;

	// Samples the texture to produce a blur in the velocity.
	vec3 sampled_colour = vec3(0.0);

	for (float i = 1.0; i < SAMPLES; ++i)
	{
		sampled_colour += texture(samplerAlbedo, fragmentUv + (i * velocity)).rgb;
	}

	outColour = vec4(sampled_colour / SAMPLES, 1.0);
}
