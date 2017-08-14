#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	float height;
	float scale;
} ubo;

layout(location = 0) in vec2 textureCoords;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = vec4(0.0);

	float part = 1.0 / ubo.height;

	vec2 blurTextureCoords[14];
	blurTextureCoords[0] = textureCoords + vec2(0.0, -part * 7.0) * ubo.scale;
	blurTextureCoords[1] = textureCoords + vec2(0.0, -part * 6.0) * ubo.scale;
	blurTextureCoords[2] = textureCoords + vec2(0.0, -part * 5.0) * ubo.scale;
	blurTextureCoords[3] = textureCoords + vec2(0.0, -part * 4.0) * ubo.scale;
	blurTextureCoords[4] = textureCoords + vec2(0.0, -part * 3.0) * ubo.scale;
	blurTextureCoords[5] = textureCoords + vec2(0.0, -part * 2.0) * ubo.scale;
	blurTextureCoords[6] = textureCoords + vec2(0.0, -part) * ubo.scale;
	blurTextureCoords[7] = textureCoords + vec2(0.0, part) * ubo.scale;
	blurTextureCoords[8] = textureCoords + vec2(0.0, part * 2.0) * ubo.scale;
	blurTextureCoords[9] = textureCoords + vec2(0.0, part * 3.0) * ubo.scale;
	blurTextureCoords[10] = textureCoords + vec2(0.0, part * 4.0) * ubo.scale;
	blurTextureCoords[11] = textureCoords + vec2(0.0, part * 5.0) * ubo.scale;
	blurTextureCoords[12] = textureCoords + vec2(0.0, part * 6.0) * ubo.scale;
	blurTextureCoords[13] = textureCoords + vec2(0.0, part * 7.0) * ubo.scale;
	outColour += texture(samplerTexture, blurTextureCoords[0]) * 0.0044299121055113265;
	outColour += texture(samplerTexture, blurTextureCoords[1]) * 0.00895781211794;
	outColour += texture(samplerTexture, blurTextureCoords[2]) * 0.0215963866053;
	outColour += texture(samplerTexture, blurTextureCoords[3]) * 0.0443683338718;
	outColour += texture(samplerTexture, blurTextureCoords[4]) * 0.0776744219933;
	outColour += texture(samplerTexture, blurTextureCoords[5]) * 0.115876621105;
	outColour += texture(samplerTexture, blurTextureCoords[6]) * 0.147308056121;
	outColour += texture(samplerTexture, textureCoords) * 0.159576912161;
	outColour += texture(samplerTexture, blurTextureCoords[7]) * 0.147308056121;
	outColour += texture(samplerTexture, blurTextureCoords[8]) * 0.115876621105;
	outColour += texture(samplerTexture, blurTextureCoords[9]) * 0.0776744219933;
	outColour += texture(samplerTexture, blurTextureCoords[10]) * 0.0443683338718;
	outColour += texture(samplerTexture, blurTextureCoords[11]) * 0.0215963866053;
	outColour += texture(samplerTexture, blurTextureCoords[12]) * 0.00895781211794;
	outColour += texture(samplerTexture, blurTextureCoords[13]) * 0.0044299121055113265;
}
