#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inCoords1;
layout(location = 1) in vec2 inCoords2;
layout(location = 2) in vec4 inColourOffset;
layout(location = 3) in float inBlendFactor;
layout(location = 4) in float inTransparency;

layout(location = 0) out vec4 outColour;

void main() {
	vec4 colour1 = texture(samplerColour, inCoords1);
	vec4 colour2 = texture(samplerColour, inCoords2);

	outColour = mix(colour1, colour2, inBlendFactor);
	outColour *= inColourOffset;
	outColour.a *= inTransparency;
}
