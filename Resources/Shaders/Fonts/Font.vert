#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UniformObject {
	mat4 modelView;
	float alpha;

	vec4 colour;
} object;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inUV;

layout(location = 0) out vec3 outUV;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = object.modelView * vec4(inPosition, 0.0f, 1.0f);

	outUV = inUV;
}
