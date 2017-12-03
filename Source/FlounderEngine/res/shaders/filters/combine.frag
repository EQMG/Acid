#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	vec4 slideSpace;
} scene;

layout(set = 0, binding = 1) uniform sampler2D samplerOriginal;
layout(set = 0, binding = 2) uniform sampler2D samplerOther;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main()
{
   if (scene.slideSpace.x <= fragmentUv.x && scene.slideSpace.y >= fragmentUv.x && 
		scene.slideSpace.z <= fragmentUv.y && scene.slideSpace.w >= fragmentUv.y) 
   {
	   outColour = texture(samplerOriginal, fragmentUv);
   } else {
	   outColour = texture(samplerOther, fragmentUv);
   }
}
