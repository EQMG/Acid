#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_JOINTS 50
#define MAX_WEIGHTS 3

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

//#if SWAY_MAPPING
//    vec2 swayOffset;
//    float swayHeight;
//#endif
} object;

//#if SWAY_MAPPING
//layout(set = 0, binding = 5) uniform sampler2D samplerSway;
//#endif

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
#ifdef NORMAL_MAPPING
layout(location = 3) in vec3 vertexTangent;
#endif
#ifdef ANIMATED
layout(location = 4) in ivec3 vertexJointIndices;
layout(location = 5) in vec3 vertexWeights;
#endif

layout(location = 0) out vec2 fragmentUv;
layout(location = 1) out vec3 fragmentNormal;
#ifdef NORMAL_MAPPING
layout(location = 2) out vec3 fragmentTangentT;
layout(location = 3) out vec3 fragmentTangentN;
layout(location = 4) out vec3 fragmentTangentB;
#endif
#ifdef TANGENT_AS_COLOUR
layout(location = 5) out vec3 fragmentColour;
#endif

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vec4 totalLocalPos = vec4(vertexPosition, 1.0f);
	vec4 totalNormal = vec4(vertexNormal, 0.0f);

#ifdef ANIMATED
    for (int i = 0; i < MAX_WEIGHTS; i++)
    {
 	    mat4 jointTransform = object.jointTransforms[vertexJointIndices[i]];
        vec4 posePosition = jointTransform * totalLocalPos;
        totalLocalPos += posePosition * vertexWeights[i];

        vec4 worldNormal = jointTransform * totalNormal;
        totalNormal += worldNormal * vertexWeights[i];
    }
#endif

//#if SWAY_MAPPING
//	vec4 swayColour = texture(samplerSway, vertexUv);
//	float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * (totalLocalPos.y / object.swayHeight) * length(totalLocalPos.xyz);
//	totalLocalPos.x += swayPower * object.swayOffset.x;
//	totalLocalPos.z += swayPower * object.swayOffset.y;
//#endif

	vec4 worldPosition = object.transform * totalLocalPos;

    gl_Position = scene.projection * scene.view * worldPosition;

    fragmentUv = vertexUv;
	fragmentNormal = normalize((object.transform * totalNormal).xyz);

#ifdef NORMAL_MAPPING
    mat3 normal_matrix = transpose(inverse(mat3(object.transform)));
    fragmentTangentT = normalize(normal_matrix * vertexTangent);
    fragmentTangentN = normalize(normal_matrix * vertexNormal);
    fragmentTangentB = normalize(cross(tangentT, tangentN));
#endif

#ifdef TANGENT_AS_COLOUR
    fragmentColour = vertexTangent;
#endif
}
