#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;

/*layout(set = 0, binding = 0) uniform UniformCamera
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} uniformCamera;*/

layout(set = 0, binding = 0) uniform UniformObject
{
	bool memes;
	float r;
	float g;
	float b;
//	vec4 colour;
//	mat4 model;
} uniformObject;

layout(location = 0) out vec3 fragmentColour;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
    gl_Position = vec4(vertexPosition, 1.0);
//	gl_Position *= uniformCamera.projection * uniformCamera.view * uniformObject.model;
    fragmentColour = vertexColour.rgb;

	if (uniformObject.memes)
	{
		fragmentColour = vec3(uniformObject.r, uniformObject.g, uniformObject.b);//uniformObject.colour.rgb;
	}
}
