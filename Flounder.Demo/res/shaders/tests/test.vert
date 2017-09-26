#version 450

#extension GL_ARB_separate_shader_objects : enable

//layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec4 vertexColour;
vec3 vertexPositions[3] = vec3[](
    vec3(0.0, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),
    vec3(-0.5, 0.5, 0.0)
);

vec4 vertexColours[3] = vec4[](
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0)
);

layout(location = 0) out vec3 fragmentColour;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vec3 vertexPosition = vertexPositions[gl_VertexIndex];
	vec4 vertexColour = vertexColours[gl_VertexIndex];

    gl_Position = vec4(vertexPosition, 1.0);
    fragmentColour = vertexColour.rgb;
}
