#version 450
#extension GL_ARB_separate_shader_objects : enable

const float EXPOSURE = 1.3;
const vec3 WHITE = vec3(1.0, 1.0, 1.0);

layout(set = 0, binding = 0) uniform sampler2D samplerColour;

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

vec3 toneMap(vec3 colour) 
{
	return colour / (1.0 + colour);
}

void main() 
{
	vec3 textureColour = texture(samplerColour, fragmentUv).rgb;
	outColour = vec4(toneMap(textureColour * EXPOSURE) / toneMap(WHITE), 1.0);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(fragmentUv * sizeColour), outColour);
}
