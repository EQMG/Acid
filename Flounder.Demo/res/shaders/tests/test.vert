#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextures;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} scene;

layout(binding = 1) uniform UboObject
{
	mat4 model;
	float swaying;
	vec2 swayOffset;
} object;

layout(binding = 3) uniform sampler2D samplerSway;

layout(location = 0) out vec2 fragmentTextures;
layout(location = 1) out vec3 fragmentNormal;

out gl_PerVertex 
{
    vec4 gl_Position;
	float gl_ClipDistance[];
};

void main() 
{
	vec4 localPosition = vec4(vertexPosition, 1.0);
	vec4 localNormal = vec4(vertexNormal, 0.0);

	if (object.swaying == 1.0)
	{
		vec4 swayColour = texture(samplerSway, vertexTextures);
		float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * length(localPosition.xyz);
		localPosition.x += swayPower * object.swayOffset.x;
		localPosition.z += swayPower * object.swayOffset.y;
	}

	vec4 worldPosition = object.model * localPosition;
	
    gl_Position = scene.projection * scene.view * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, scene.clip);

    fragmentTextures = vertexTextures;
	fragmentNormal = normalize((object.model * localNormal).xyz);
}
