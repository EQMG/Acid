#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D samplerTexture;

layout(binding = 1) uniform UBO 
{
	bool polygonMode;
} ubo;

layout(location = 0) in vec2 textureCoords1;
layout(location = 1) in vec2 textureCoords2;
layout(location = 2) in float textureBlendFactor;
layout(location = 3) in float textureTransparency;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormals;
layout(location = 2) out vec4 outExtras;

//---------MAIN------------
void main(void) 
{
	vec4 colour1 = texture(samplerTexture, textureCoords1);
	vec4 colour2 = texture(samplerTexture, textureCoords2);

	outAlbedo = mix(colour1, colour2, textureBlendFactor);
	outAlbedo.a -= textureTransparency;
	
	if (ubo.polygonMode) 
	{
		outAlbedo = vec4(1.0, 0.0, 0.0, 1.0);
	}

	if (outAlbedo.a <= 0.2) 
	{
		outAlbedo = vec4(0.0);
		outNormals = vec4(0.0);
		outExtras = vec4(0.0);
		discard;
	}

	outNormals = vec4(0.0, 1.0, 0.0, 1.0);
	outExtras = vec4(1.0, 0.0, 2.0 / 3.0, 1.0);
}
