#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(push_constant) uniform PushScene {
	float innerRadius;
	float outerRadius;
	float opacity;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	vec4 textureColour = texture(samplerColour, inUV);
	vec4 colour = textureColour;
	colour.rgb *= 1.0f - smoothstep(scene.innerRadius, scene.outerRadius, length(inUV - 0.5f));
	colour = mix(textureColour, colour, scene.opacity);
	
	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
