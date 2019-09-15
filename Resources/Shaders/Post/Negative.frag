#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() {
	vec3 textureColour = texture(samplerColour, inUV).rgb;
	vec4 colour = vec4(1.0f - textureColour, 1.0f);
	
	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
