#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles, invocations = NUM_CASCADES) in;
layout (triangle_strip, max_vertices = 3) out;

layout(set = 0, binding = 0) uniform UboScene
{
    mat4 projectionView[NUM_CASCADES];
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 instancePos[3];
} object;

layout (location = 0) in int inInstanceIndex[];

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	vec4 instancedPos = object.instancePos[inInstanceIndex[0]];

	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		vec4 tmpPos = gl_in[i].gl_Position + instancedPos;
		gl_Position = object.transform * scene.projectionView[gl_InvocationID] * tmpPos;
		EmitVertex();
	}

	EndPrimitive();
}