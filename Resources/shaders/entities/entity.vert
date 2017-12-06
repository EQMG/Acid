#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	float swaying;

	float metallic;
	float roughness;

	vec2 swayOffset;
} object;

layout(set = 0, binding = 3) uniform sampler2D samplerSway;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(location = 0) out vec2 fragmentUv;
layout(location = 1) out vec3 fragmentNormal;

out gl_PerVertex 
{
    vec4 gl_Position;
	float gl_ClipDistance[];
};

void main() 
{
	vec4 localPosition = vec4(vertexPosition, 1.0f);
	vec4 localNormal = vec4(vertexNormal, 0.0f);

	if (object.swaying == 1.0f)
	{
		vec4 swayColour = texture(samplerSway, vertexUv);
		float swayPower = 0.5f * exp(log(length(swayColour.rgb)) / 3.0f) * length(localPosition.xyz);
		localPosition.x += swayPower * object.swayOffset.x;
		localPosition.z += swayPower * object.swayOffset.y;
	}

	vec4 worldPosition = object.transform * localPosition;
	
    gl_Position = scene.projection * scene.view * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, scene.clip);

    fragmentUv = vertexUv;
	fragmentNormal = normalize((object.transform * localNormal).xyz);
}
