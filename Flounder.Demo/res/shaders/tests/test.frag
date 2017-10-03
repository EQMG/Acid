#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragmentTextures;
layout(location = 1) in vec3 fragmentNormal;

layout(binding = 1) uniform sampler2D samplerTexture;

layout(location = 0) out vec4 outColour;

const vec2 lightBias = vec2(0.7, 0.1);
const vec3 lightDirection = vec3(0.2, -0.3, 0.8);

void main() 
{
	vec3 unitNormal = normalize(fragmentNormal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;

	vec3 textureColour = texture(samplerTexture, fragmentTextures).rgb;

	outColour = vec4(textureColour * diffuseLight, 1.0);
}
