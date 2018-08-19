#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	float height;
	float scale;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main()
{
	float part = 1.0f / scene.height;

	vec2 blurTextureCoords[14];
	blurTextureCoords[0] = inUv + vec2(0.0f, -part * 7.0f) * scene.scale;
	blurTextureCoords[1] = inUv + vec2(0.0f, -part * 6.0f) * scene.scale;
	blurTextureCoords[2] = inUv + vec2(0.0f, -part * 5.0f) * scene.scale;
	blurTextureCoords[3] = inUv + vec2(0.0f, -part * 4.0f) * scene.scale;
	blurTextureCoords[4] = inUv + vec2(0.0f, -part * 3.0f) * scene.scale;
	blurTextureCoords[5] = inUv + vec2(0.0f, -part * 2.0f) * scene.scale;
	blurTextureCoords[6] = inUv + vec2(0.0f, -part) * scene.scale;
	blurTextureCoords[7] = inUv + vec2(0.0f, part) * scene.scale;
	blurTextureCoords[8] = inUv + vec2(0.0f, part * 2.0f) * scene.scale;
	blurTextureCoords[9] = inUv + vec2(0.0f, part * 3.0f) * scene.scale;
	blurTextureCoords[10] = inUv + vec2(0.0f, part * 4.0f) * scene.scale;
	blurTextureCoords[11] = inUv + vec2(0.0f, part * 5.0f) * scene.scale;
	blurTextureCoords[12] = inUv + vec2(0.0f, part * 6.0f) * scene.scale;
	blurTextureCoords[13] = inUv + vec2(0.0f, part * 7.0f) * scene.scale;

	outColour = vec4(0.0f);
	outColour += texture(samplerColour, blurTextureCoords[0]) * 0.0044299121055113265f;
	outColour += texture(samplerColour, blurTextureCoords[1]) * 0.00895781211794f;
	outColour += texture(samplerColour, blurTextureCoords[2]) * 0.0215963866053f;
	outColour += texture(samplerColour, blurTextureCoords[3]) * 0.0443683338718f;
	outColour += texture(samplerColour, blurTextureCoords[4]) * 0.0776744219933f;
	outColour += texture(samplerColour, blurTextureCoords[5]) * 0.115876621105f;
	outColour += texture(samplerColour, blurTextureCoords[6]) * 0.147308056121f;
	outColour += texture(samplerColour, inUv) * 0.159576912161f;
	outColour += texture(samplerColour, blurTextureCoords[7]) * 0.147308056121f;
	outColour += texture(samplerColour, blurTextureCoords[8]) * 0.115876621105f;
	outColour += texture(samplerColour, blurTextureCoords[9]) * 0.0776744219933f;
	outColour += texture(samplerColour, blurTextureCoords[10]) * 0.0443683338718f;
	outColour += texture(samplerColour, blurTextureCoords[11]) * 0.0215963866053f;
	outColour += texture(samplerColour, blurTextureCoords[12]) * 0.00895781211794f;
	outColour += texture(samplerColour, blurTextureCoords[13]) * 0.0044299121055113265f;

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
