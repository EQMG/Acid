#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	vec4 diffuse;

	vec4 samples;

	vec3 surface;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerDiffuse;
layout(set = 0, binding = 3) uniform sampler2D samplerMaterial;
layout(set = 0, binding = 4) uniform sampler2D samplerNormal;

layout(location = 0) in vec2 fragmentUv;
layout(location = 1) in vec3 fragmentNormal;
layout(location = 2) in mat3 tangentSpace;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

vec2 encodeNormal(vec3 normal)
{
	vec2 result = vec2(0.0f);
	result.x = (atan(normal.y, normal.x) / 3.14159f) * 0.5f + 0.5f;
	result.y = normal.z * 0.5f + 0.5f;
	return result;
}

void main() 
{
	vec4 textureColour = object.diffuse;
	vec3 unitNormal = normalize(fragmentNormal);
	vec3 material = vec3(object.surface.x, object.surface.y, 0.0f);

	if (object.samples.x == 1.0f)
	{
	    textureColour = texture(samplerDiffuse, fragmentUv);
	}

	if (object.samples.y == 1.0f)
	{
	    vec4 textureMaterial = texture(samplerMaterial, fragmentUv);
	    material.x *= textureMaterial.r;
	    material.y *= textureMaterial.g;
	    material.z = textureMaterial.b;
	}

	if (object.samples.z == 1.0f)
	{
	    vec4 textureNormal = texture(samplerNormal, fragmentUv);
	    unitNormal = textureNormal.rgb;
        unitNormal = normalize(textureNormal.rgb * 2.0f - vec3(1.0f));
        unitNormal = normalize(tangentSpace * unitNormal);
	}


	outColour = textureColour;
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(material.x, material.y, object.surface.z, 1.0f);
}
