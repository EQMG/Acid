#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
#ifdef ANIMATED
    mat4 jointTransforms[MAX_JOINTS];
#endif
	mat4 transform;

	vec4 baseColor;
	float metallic;
	float roughness;
	float ignoreFog;
	float ignoreLighting;
} object;

layout(set = 0, location = 0) in vec3 vertexPosition;
layout(set = 0, location = 1) in vec2 vertexUv;
layout(set = 0, location = 2) in vec3 vertexNormal;
layout(set = 0, location = 3) in vec3 vertexTangent;
#ifdef ANIMATED
layout(set = 0, location = 4) in vec3 vertexJointIds;
layout(set = 0, location = 5) in vec3 vertexWeights;
#endif

layout(location = 0) out vec2 fragmentUv;
layout(location = 1) out vec3 fragmentNormal;
#ifdef NORMAL_MAPPING
layout(location = 2) out mat3 tangent;
#endif

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
#ifdef ANIMATED
    vec4 totalLocalPos = vec4(0.0f);
    vec4 totalNormal = vec4(0.0f);

    for (int i = 0; i < MAX_WEIGHTS; i++)
    {
        mat4 jointTransform = object.jointTransforms[int(vertexJointIds[i])];
        vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0f);
        totalLocalPos += posePosition * vertexWeights[i];

        vec4 worldNormal = jointTransform * vec4(vertexNormal, 0.0f);
        totalNormal += worldNormal * vertexWeights[i];
    }
#else
	vec4 totalLocalPos = vec4(vertexPosition, 1.0f);
	vec4 totalNormal = vec4(vertexNormal, 0.0f);
#endif

	vec4 worldPosition = object.transform * totalLocalPos;

    gl_Position = scene.projection * scene.view * worldPosition;

    fragmentUv = vertexUv;
	fragmentNormal = normalize((object.transform * totalNormal).xyz);

#ifdef NORMAL_MAPPING
    mat3 normalMatrix = transpose(inverse(mat3(object.transform)));
    vec3 tangentT = normalize(normalMatrix * vertexTangent);
    vec3 tangentN = normalize(normalMatrix * vertexNormal);
    vec3 tangentB = normalize(cross(tangentT, tangentN));
    tangent = mat3(tangentT, tangentN, tangentB);
#endif
}
