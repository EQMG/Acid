#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	float pixelSize;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 2) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main() 
{
	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	vec2 coord;

	if (inUv.x < 1.0f)
	{
		float dx = scene.pixelSize * (1.0f / sizeAlbedo.x);
		float dy = scene.pixelSize * (1.0f / sizeAlbedo.y);
		coord.x = dx * floor(inUv.x / dx);
		coord.y = dy * floor(inUv.y / dy);
	} 
	else if (inUv.x > 1.0)
	{
		coord = inUv;
	}

	vec3 albedo = texture(samplerAlbedo, coord).rgb;
	outAlbedo = vec4(albedo, 1.0f);

	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
