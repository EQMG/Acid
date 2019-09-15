#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform UniformObject {
	mat4 transform;
} object;

layout(binding = 2) uniform sampler2D samplerR;
layout(binding = 3) uniform sampler2D samplerG;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() {
	//vec4 diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//diffuse.rgb += inTexture.r * texture(samplerR, inUV).rgb;
	//diffuse.rgb += inTexture.g * texture(samplerG, inUV).rgb;
	vec4 diffuse = texture(samplerR, inUV);
	
	outPosition = vec4(inPosition, 1.0f);
	outDiffuse = diffuse;
	outNormal = vec4(normalize(inNormal), 1.0f);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
