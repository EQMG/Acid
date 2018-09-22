#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

const float innerRadius = 0.15f;
const float outerRadius = 1.35f;
const float opacity = 0.85f;

void main() 
{
	vec4 textureColour = texture(samplerColour, inUv);
    outColour.rgb *= 1.0f - smoothstep(innerRadius, outerRadius, length(inUv));
	outColour = mix(textureColour, outColour, opacity);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
