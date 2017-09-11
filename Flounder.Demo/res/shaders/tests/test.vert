#version 450

#extension GL_ARB_separate_shader_objects : enable

/*layout(binding = 0) uniform UBO 
{
	vec3 colour;
} ubo;*/

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColour;

/*vec2 inPosition[3] = vec2[]
(
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

vec3 inColour[3] = vec3[]
(
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);*/ // [gl_VertexIndex]

layout(location = 0) out vec3 fragColour;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = vec4(inPosition, 0.0, 1.0);
	fragColour = inColour;
}