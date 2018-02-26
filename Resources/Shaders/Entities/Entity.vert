#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	vec4 baseColor;

	vec4 samples;

	vec4 surface;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

layout(location = 0) out vec2 fragmentUv;
layout(location = 1) out vec3 fragmentNormal;
layout(location = 2) out mat3 tangentSpace;

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vec4 localPosition = vec4(vertexPosition, 1.0f);
	vec4 localNormal = vec4(vertexNormal, 0.0f);
	vec4 worldPosition = object.transform * localPosition;
	
    gl_Position = scene.projection * scene.view * worldPosition;

    fragmentUv = vertexUv;
	fragmentNormal = normalize((object.transform * localNormal).xyz);

	if (object.samples.z == 1.0f)
	{
        mat3 normal_matrix = transpose(inverse(mat3(object.transform)));
        vec3 T = normalize(normal_matrix * vertexTangent);
        vec3 N = normalize(normal_matrix * vertexNormal);
        vec3 B = normalize(cross(T, N));
        tangentSpace = mat3(T, B, N);
	}
}
