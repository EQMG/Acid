#version 450
#extension GL_ARB_separate_shader_objects : enable

const float SAMPLES = 8.0;

layout(set = 0, binding = 0) uniform UboScene 
{
	mat4 projection;
	mat4 view;
	mat4 lastView;
	float delta;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerDepth;
layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 2) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

vec3 decodeWorldPosition(vec2 uv, float depth) 
{
	vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0);
	return p.xyz / p.w;
}

void main() 
{
	float textureDepth = texture(samplerDepth, fragmentUv).r;

	// Gets the currebt world position for this fragment.
	vec4 worldPosition = vec4(decodeWorldPosition(fragmentUv, textureDepth.r), 1.0);

	// Transform by the view-projection inverse to get the current world position.
	vec4 currentPos = scene.projection * scene.view * worldPosition;

	// Use the world position, and transform by the previous view-projection matrix.
	vec4 previousPos = scene.projection * scene.lastView * worldPosition;

	// Use this frame's position and last frame's to compute the pixel velocity.
	vec2 velocity = (currentPos.xy - previousPos.xy) * 0.02 * scene.delta;

	// Samples the texture to produce a blur in the velocity.
	vec3 sampledColour = vec3(0.0);

	for (float i = 1.0; i < SAMPLES; ++i)
	{
		sampledColour += texture(samplerColour, fragmentUv + (i * velocity)).rgb;
	}

	outColour = vec4(sampledColour / SAMPLES, 1.0);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
