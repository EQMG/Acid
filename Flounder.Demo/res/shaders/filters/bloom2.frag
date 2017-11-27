#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform sampler2D samplerColour;
layout(set = 0, binding = 1) uniform sampler2D samplerBloom1;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec4 textureColour = texture(samplerColour, fragmentUv);
	vec4 textureBloom1 = texture(samplerBloom1, fragmentUv);
	outColour = vec4(textureColour.rgb + textureBloom1.rgb, 1.0f);
}
