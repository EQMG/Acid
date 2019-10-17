#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UniformObject {
	mat4 modelView;
	float alpha;

	float aspectRatio;

	vec4 colourOffset;
	vec2 atlasOffset;
	vec2 atlasScale;
	float atlasRows;
	vec4 ninePatches;
} object;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = object.modelView * vec4(inPosition, 0.0f, 1.0f);

	outUV = object.atlasScale * ((inUV.xy / object.atlasRows) + object.atlasOffset);
}
