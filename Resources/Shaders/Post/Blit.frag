#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

void main() {
	outColour = texture(samplerColour, inUV);
}
