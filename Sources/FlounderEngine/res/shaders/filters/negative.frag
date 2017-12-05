#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec3 colour = texture(samplerColour, fragmentUv).rgb;
	outColour = vec4((1.0 - colour), 1.0);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
