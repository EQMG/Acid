#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(triangles, invocations = NUM_CASCADES) in;
layout(triangle_strip, max_vertices = 3) out;

layout(set = 0, binding = 0) uniform UboScene
{
    mat4 cropMatrix[NUM_CASCADES];
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

//layout(location = 1) out int outSplit;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	for (int i = 0; i < NUM_CASCADES; i++) // TODO: gl_in.length() (Might be two loops?)
	{
		vec4 worldPosition = object.transform * gl_in[i].gl_Position;
		gl_Position = scene.cropMatrix[i] * worldPosition;
	//	outSplit = i;
		EmitVertex();
	}

	EndPrimitive();
}