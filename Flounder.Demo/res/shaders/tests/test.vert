#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextures;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(set = 0, binding = 0) uniform UBO
{
	mat4 projection;
	mat4 view;
	vec4 clip;
	mat4 model;
} ubo;

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
	vec4 worldPosition = ubo.model * localPosition;
	
    gl_Position = ubo.projection * ubo.view * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, ubo.clip);

    fragmentTextures = vertexTextures;
	fragmentNormal = normalize((ubo.model * localNormal).xyz);
}
