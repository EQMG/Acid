#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec3 textureColour = texture(samplerColour, inUv).rgb;
	float grey = dot(textureColour, vec3(0.299f, 0.587f, 0.114f));
	outColour = vec4(grey * vec3(1.2f, 1.0f, 0.8f), 1.0f);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
