#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform sampler2D samplerColour;

layout(set = 0, binding = 1) uniform UBO 
{
	float pixelSize;
	vec2 displaySize;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec2 coord;

	if (fragmentUv.x < 1.0) 
	{
		float dx = ubo.pixelSize * (1.0 / ubo.displaySize.x);
		float dy = ubo.pixelSize * (1.0 / ubo.displaySize.y);
		coord.x = dx * floor(fragmentUv.x / dx);
		coord.y = dy * floor(fragmentUv.y / dy);
	} 
	else if (fragmentUv.x > 1.0) 
	{
		coord = fragmentUv;
	}

	vec3 colour = texture(samplerColour, coord).rgb;
	outColour = vec4(colour, 1.0);
}
