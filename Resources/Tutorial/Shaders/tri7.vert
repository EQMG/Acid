#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformScene {
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
} scene;

layout(binding = 1) uniform UniformObject {
	mat4 transform;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec2 fragTexCoord;
//layout(location = 1) out vec3 fragColor;

void main() {
    gl_Position = scene.projection * scene.view * object.transform * vec4(inPosition, 1.0);
   // fragColor = inColor;
    fragTexCoord = inTexCoord;
}