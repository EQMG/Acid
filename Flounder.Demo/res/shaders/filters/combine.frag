#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerOriginal;
layout(binding = 1) uniform sampler2D samplerOther;

layout(binding = 2) uniform UBO 
{
	vec4 slideSpace;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void)
{
   if (ubo.slideSpace.x <= fragmentUv.x && ubo.slideSpace.y >= fragmentUv.x && 
		ubo.slideSpace.z <= fragmentUv.y && ubo.slideSpace.w >= fragmentUv.y) 
   {
	   outColour = texture(samplerOriginal, fragmentUv);
   } else {
	   outColour = texture(samplerOther, fragmentUv);
   }
}
