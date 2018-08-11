#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
    float width;
    float scale;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeAlbedo;

layout(set = 0, binding = 2) uniform sampler2D samplerAlbedo;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outAlbedo;

void main()
{
	float part = 1.0f / scene.width;

	vec2 blurTextureCoords[14];
	blurTextureCoords[0] = inUv + vec2(-part * 7.0f, 0.0f) * scene.scale;
	blurTextureCoords[1] = inUv + vec2(-part * 6.0f, 0.0f) * scene.scale;
	blurTextureCoords[2] = inUv + vec2(-part * 5.0f, 0.0f) * scene.scale;
	blurTextureCoords[3] = inUv + vec2(-part * 4.0f, 0.0f) * scene.scale;
	blurTextureCoords[4] = inUv + vec2(-part * 3.0f, 0.0f) * scene.scale;
	blurTextureCoords[5] = inUv + vec2(-part * 2.0f, 0.0f) * scene.scale;
	blurTextureCoords[6] = inUv + vec2(-part, 0.0f) * scene.scale;
	blurTextureCoords[7] = inUv + vec2(part, 0.0f) * scene.scale;
	blurTextureCoords[8] = inUv + vec2(part * 2.0f, 0.0f) * scene.scale;
	blurTextureCoords[9] = inUv + vec2(part * 3.0f, 0.0f) * scene.scale;
	blurTextureCoords[10] = inUv + vec2(part * 4.0f, 0.0f) * scene.scale;
	blurTextureCoords[11] = inUv + vec2(part * 5.0f, 0.0f) * scene.scale;
	blurTextureCoords[12] = inUv + vec2(part * 6.0f, 0.0f) * scene.scale;
	blurTextureCoords[13] = inUv + vec2(part * 7.0f, 0.0f) * scene.scale;

	outAlbedo = vec4(0.0f);
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[0]) * 0.0044299121055113265f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[1]) * 0.00895781211794f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[2]) * 0.0215963866053f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[3]) * 0.0443683338718f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[4]) * 0.0776744219933f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[5]) * 0.115876621105f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[6]) * 0.147308056121f;
	outAlbedo += texture(samplerAlbedo, inUv) * 0.159576912161f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[7]) * 0.147308056121f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[8]) * 0.115876621105f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[9]) * 0.0776744219933f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[10]) * 0.0443683338718f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[11]) * 0.0215963866053f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[12]) * 0.00895781211794f;
	outAlbedo += texture(samplerAlbedo, blurTextureCoords[13]) * 0.0044299121055113265f;

	vec2 sizeAlbedo = textureSize(samplerAlbedo, 0);
	imageStore(writeAlbedo, ivec2(inUv * sizeAlbedo), outAlbedo);
}
