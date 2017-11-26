#version 450

#extension GL_ARB_separate_shader_objects : enable

const float EXPOSURE = 1.3;
const vec3 WHITE = vec3(1.0, 1.0, 1.0);

layout(binding = 0) uniform sampler2D samplerColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

vec3 toneMap(vec3 colour) 
{
	return colour / (1.0 + colour);
}

void main(void) 
{
	vec3 colour = texture(samplerColour, fragmentUv).rgb;
	outColour = vec4(toneMap(colour * EXPOSURE) / toneMap(WHITE), 1.0);
}
