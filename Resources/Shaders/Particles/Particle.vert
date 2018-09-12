#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
} scene;

// Vertex attributes
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

// Instanced attributes
layout(location = 4) in mat4 inMvp;
layout(location = 8) in vec3 inColourOffset;
layout(location = 9) in vec4 inOffsets;
layout(location = 10) in vec3 inBlend;

layout(location = 0) out vec2 outCoords1;
layout(location = 1) out vec2 outCoords2;
layout(location = 2) out vec3 outColourOffset;
layout(location = 3) out float outBlendFactor;
layout(location = 4) out float outTransparency;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = inMvp * vec4(inPosition, 1.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

	vec2 uv = inPosition.xy + vec2(0.5f, 0.5f);
	uv.y = 1.0f - uv.y;
	uv /= inBlend.z;

    outColourOffset = inColourOffset;
	outCoords1 = uv + inOffsets.xy;
	outCoords2 = uv + inOffsets.zw;
	outBlendFactor = inBlend.x;
	outTransparency = inBlend.y;
}
