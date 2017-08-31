#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UBO 
{
	vec3 colour;
} ubo;

vec2 positions[3] = vec2[]
(
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[]
(
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 fragColour;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	fragColour = colors[gl_VertexIndex];
}