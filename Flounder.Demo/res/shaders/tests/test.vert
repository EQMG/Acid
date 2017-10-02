#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec3 vertexNormal;

layout(set = 0, binding = 0) uniform UBO
{
	mat4 projection;
	mat4 view;
	vec4 clip;
	mat4 model;
} ubo;

layout(location = 0) out vec3 fragmentColour;

const vec2 lightBias = vec2(0.7, 0.6); // Just indicates the balance between diffuse and ambient lighting.

out gl_PerVertex 
{
    vec4 gl_Position;
//	float gl_ClipDistance[];
};

void main() 
{
	vec4 localPosition = vec4(vertexPosition, 1.0);
	vec4 worldPosition = ubo.model * localPosition;
	
//	gl_ClipDistance[0] = dot(worldPosition, ubo.clip);
    gl_Position = ubo.projection * ubo.view * worldPosition;

    fragmentColour = vertexColour.rgb;
}
