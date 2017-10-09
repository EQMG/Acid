#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	float shineDamper;
	float reflectivity;
} object;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentTextures;
layout(location = 2) in vec3 fragmentColour;

layout(location = 0) out vec4 outColour;
//layout(location = 1) out vec4 outNormals;
//layout(location = 2) out vec4 outExtras;

const vec2 lightBias = vec2(0.8, 0.2);
const vec3 lightDirection = vec3(0.2, -0.3, 0.2);

void main(void) 
{
	vec3 unitNormal = normalize(fragmentNormal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;

	outColour = vec4(fragmentColour * diffuseLight, 1.0);
//	outNormals = vec4(fragmentNormal + 1.0 / 2.0, out_albedo.a);
//	outExtras = vec4(object.shineDamper, object.reflectivity, 0.0, out_albedo.a);
}