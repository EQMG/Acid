#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
    float innerRadius;
    float outerRadius;
    float opacity;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec4 textureColour = texture(samplerColour, inUv);
	outColour = textureColour;
    outColour.rgb *= 1.0f - smoothstep(scene.innerRadius, scene.outerRadius, length(inUv - 0.5f));
	outColour = mix(textureColour, outColour, scene.opacity);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
