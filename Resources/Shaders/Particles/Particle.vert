#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UniformScene {
	mat4 projection;
	mat4 view;
} scene;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 3) in mat4 inModelMatrix;
layout(location = 7) in vec4 inColourOffset;
layout(location = 8) in vec4 inOffsets;
layout(location = 9) in vec3 inBlend;

layout(location = 0) out vec2 outCoords1;
layout(location = 1) out vec2 outCoords2;
layout(location = 2) out vec4 outColourOffset;
layout(location = 3) out float outBlendFactor;
layout(location = 4) out float outTransparency;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	vec4 worldPosition = inModelMatrix * vec4(inPosition, 1.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

	vec2 uv = inUV / inBlend.z;

	outColourOffset = inColourOffset;
	outCoords1 = uv + inOffsets.xy;
	outCoords2 = uv + inOffsets.zw;
	outBlendFactor = inBlend.x;
	outTransparency = inBlend.y;
}
