#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerColour;

layout(binding = 1) uniform UBO 
{
	float width;
	float scale;
} ubo;

layout(location = 0) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = vec4(0.0);

	float part = 1.0 / ubo.width;

	vec2 blurfragmentUv[14];
	blurfragmentUv[0] = fragmentUv + vec2(-part * 7.0, 0.0) * ubo.scale;
	blurfragmentUv[1] = fragmentUv + vec2(-part * 6.0, 0.0) * ubo.scale;
	blurfragmentUv[2] = fragmentUv + vec2(-part * 5.0, 0.0) * ubo.scale;
	blurfragmentUv[3] = fragmentUv + vec2(-part * 4.0, 0.0) * ubo.scale;
	blurfragmentUv[4] = fragmentUv + vec2(-part * 3.0, 0.0) * ubo.scale;
	blurfragmentUv[5] = fragmentUv + vec2(-part * 2.0, 0.0) * ubo.scale;
	blurfragmentUv[6] = fragmentUv + vec2(-part, 0.0) * ubo.scale;
	blurfragmentUv[7] = fragmentUv + vec2(part, 0.0) * ubo.scale;
	blurfragmentUv[8] = fragmentUv + vec2(part * 2.0, 0.0) * ubo.scale;
	blurfragmentUv[9] = fragmentUv + vec2(part * 3.0, 0.0) * ubo.scale;
	blurfragmentUv[10] = fragmentUv + vec2(part * 4.0, 0.0) * ubo.scale;
	blurfragmentUv[11] = fragmentUv + vec2(part * 5.0, 0.0) * ubo.scale;
	blurfragmentUv[12] = fragmentUv + vec2(part * 6.0, 0.0) * ubo.scale;
	blurfragmentUv[13] = fragmentUv + vec2(part * 7.0, 0.0) * ubo.scale;
	outColour += texture(samplerColour, blurfragmentUv[0]) * 0.0044299121055113265;
	outColour += texture(samplerColour, blurfragmentUv[1]) * 0.00895781211794;
	outColour += texture(samplerColour, blurfragmentUv[2]) * 0.0215963866053;
	outColour += texture(samplerColour, blurfragmentUv[3]) * 0.0443683338718;
	outColour += texture(samplerColour, blurfragmentUv[4]) * 0.0776744219933;
	outColour += texture(samplerColour, blurfragmentUv[5]) * 0.115876621105;
	outColour += texture(samplerColour, blurfragmentUv[6]) * 0.147308056121;
	outColour += texture(samplerColour, fragmentUv) * 0.159576912161;
	outColour += texture(samplerColour, blurfragmentUv[7]) * 0.147308056121;
	outColour += texture(samplerColour, blurfragmentUv[8]) * 0.115876621105;
	outColour += texture(samplerColour, blurfragmentUv[9]) * 0.0776744219933;
	outColour += texture(samplerColour, blurfragmentUv[10]) * 0.0443683338718;
	outColour += texture(samplerColour, blurfragmentUv[11]) * 0.0215963866053;
	outColour += texture(samplerColour, blurfragmentUv[12]) * 0.00895781211794;
	outColour += texture(samplerColour, blurfragmentUv[13]) * 0.0044299121055113265;
}
