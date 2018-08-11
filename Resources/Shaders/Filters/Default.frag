#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 1) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main() 
{
	outAlbedo = texture(samplerAlbedo, inUv);

	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
