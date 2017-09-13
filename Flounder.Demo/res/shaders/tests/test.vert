#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColour;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex 
{
    vec4 gl_Position;
};


void main() 
{
    gl_Position = vec4(inPosition, 1.0);
    fragColor = inColour;
}

/*#version 450

#extension GL_ARB_separate_shader_objects : enable

vec2 inPosition[3] = vec2[]
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
);

layout(location = 0) out vec3 fragColour;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	gl_Position = vec4(inPosition[gl_VertexIndex], 0.0, 1.0);
	fragColour = inColour[gl_VertexIndex];
}*/