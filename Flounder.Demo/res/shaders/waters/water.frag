#version 450

#extension GL_ARB_separate_shader_objects : enable

//layout(binding = 2) uniform sampler2D samplerReflections;

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 waterOffset;

	float waveTime;
	float waveLength;
	float amplitude;
	float squareSize;
	
	vec4 diffuseColour;

	float shineDamper;
	float reflectivity;

	float ignoreReflections;
} object;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec4 fragmentClipSpace;

layout(location = 0) out vec4 outColour;
//layout(location = 1) out vec4 outNormals;
//layout(location = 2) out vec4 outExtras;

const vec2 lightBias = vec2(0.7, 0.1);
const vec3 lightDirection = vec3(0.2, -0.3, 0.8);

//---------REFRACTION------------
vec2 getReflectionTexCoords(vec2 normalizedDeviceCoords)
{
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x, -normalizedDeviceCoords.y);
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	return reflectionTextureCoords;
}

//---------MAIN------------
void main(void) 
{
	vec3 unitNormal = normalize(fragmentNormal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;

	//if (object.ignoreReflections != 0.0) 
	//{
	//	vec2 normalizedDeviceCoords = (fragmentClipSpace.xy / fragmentClipSpace.w) / 2.0 + 0.5;
	//	vec2 reflectionTextureCoords = getReflectionTexCoords(normalizedDeviceCoords);
	//	vec3 reflectionColour = texture(samplerReflections, reflectionTextureCoords).rgb;
	//	outColour = vec4(mix(reflectionColour, object.diffuseColour.rgb, object.diffuseColour.a), 1.0);
	//} 
	//else 
	//{
		outColour = vec4(object.diffuseColour.rgb * diffuseLight, 1.0);
	//}
	
//	outNormals = vec4(fragmentNormal + 1.0 / 2.0, 1.0);
//	outExtras = vec4(object.shineDamper, object.reflectivity, (1.0 / 3.0) * (float(false) + 2.0 * float(false)), 1.0);
}