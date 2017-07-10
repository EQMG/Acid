#version

//---------IN------------
in vec2 pass_textureCoords;

//---------UNIFORM------------
layout(binding = 0) uniform sampler2D originalTexture;
uniform float height;
uniform float scale;

//---------OUT------------
layout(location = 0) out vec4 out_colour;

//---------MAIN------------
void main(void) {
	out_colour = vec4(0.0);

	float part = 1.0 / height;

	vec2 pass_blurTextureCoords[14];
	pass_blurTextureCoords[0] = pass_textureCoords + vec2(0.0, -part * 7.0) * scale;
	pass_blurTextureCoords[1] = pass_textureCoords + vec2(0.0, -part * 6.0) * scale;
	pass_blurTextureCoords[2] = pass_textureCoords + vec2(0.0, -part * 5.0) * scale;
	pass_blurTextureCoords[3] = pass_textureCoords + vec2(0.0, -part * 4.0) * scale;
	pass_blurTextureCoords[4] = pass_textureCoords + vec2(0.0, -part * 3.0) * scale;
	pass_blurTextureCoords[5] = pass_textureCoords + vec2(0.0, -part * 2.0) * scale;
	pass_blurTextureCoords[6] = pass_textureCoords + vec2(0.0, -part) * scale;
	pass_blurTextureCoords[7] = pass_textureCoords + vec2(0.0, part) * scale;
	pass_blurTextureCoords[8] = pass_textureCoords + vec2(0.0, part * 2.0) * scale;
	pass_blurTextureCoords[9] = pass_textureCoords + vec2(0.0, part * 3.0) * scale;
	pass_blurTextureCoords[10] = pass_textureCoords + vec2(0.0, part * 4.0) * scale;
	pass_blurTextureCoords[11] = pass_textureCoords + vec2(0.0, part * 5.0) * scale;
	pass_blurTextureCoords[12] = pass_textureCoords + vec2(0.0, part * 6.0) * scale;
	pass_blurTextureCoords[13] = pass_textureCoords + vec2(0.0, part * 7.0) * scale;
	out_colour += texture(originalTexture, pass_blurTextureCoords[0]) * 0.0044299121055113265;
	out_colour += texture(originalTexture, pass_blurTextureCoords[1]) * 0.00895781211794;
	out_colour += texture(originalTexture, pass_blurTextureCoords[2]) * 0.0215963866053;
	out_colour += texture(originalTexture, pass_blurTextureCoords[3]) * 0.0443683338718;
	out_colour += texture(originalTexture, pass_blurTextureCoords[4]) * 0.0776744219933;
	out_colour += texture(originalTexture, pass_blurTextureCoords[5]) * 0.115876621105;
	out_colour += texture(originalTexture, pass_blurTextureCoords[6]) * 0.147308056121;
	out_colour += texture(originalTexture, pass_textureCoords) * 0.159576912161;
	out_colour += texture(originalTexture, pass_blurTextureCoords[7]) * 0.147308056121;
	out_colour += texture(originalTexture, pass_blurTextureCoords[8]) * 0.115876621105;
	out_colour += texture(originalTexture, pass_blurTextureCoords[9]) * 0.0776744219933;
	out_colour += texture(originalTexture, pass_blurTextureCoords[10]) * 0.0443683338718;
	out_colour += texture(originalTexture, pass_blurTextureCoords[11]) * 0.0215963866053;
	out_colour += texture(originalTexture, pass_blurTextureCoords[12]) * 0.00895781211794;
	out_colour += texture(originalTexture, pass_blurTextureCoords[13]) * 0.0044299121055113265;
}
