#version 450
#extension GL_ARB_separate_shader_objects : enable

#if ANIMATED
const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;
#endif

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

#if ANIMATED
    mat4 jointTransforms[MAX_JOINTS];
#endif

#if SWAY_MAPPING
    vec2 swayOffset;
    float swayHeight;
#endif

	vec4 baseColor;

	float metallic;
	float roughness;
	float ignoreFog;
	float ignoreLighting;
} object;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
#if ANIMATED
layout(location = 4) in ivec3 vertexJointIndices;
layout(location = 5) in vec3 vertexWeights;
#endif

#if SWAY_MAPPING
layout(set = 0, binding = 5) uniform sampler2D samplerSway;
#endif

layout(location = 0) out vec2 fragmentUv;
layout(location = 1) out vec3 fragmentNormal;
#if TANGENT_AS_COLOUR
layout(location = 2) out vec3 fragmentColour;
#endif
#if NORMAL_MAPPING
layout(location = 2) out vec3 tangentT;
layout(location = 3) out vec3 tangentN;
layout(location = 4) out vec3 tangentB;
#endif

out gl_PerVertex 
{
    vec4 gl_Position;
};

void main() 
{
	vec4 totalLocalPos = vec4(vertexPosition, 1.0f);
	vec4 totalNormal = vec4(vertexNormal, 0.0f);

#if ANIMATED
   for (int i = 0; i < MAX_WEIGHTS; i++)
   {
	    mat4 jointTransform = object.jointTransforms[vertexJointIndices[i]];
        vec4 posePosition = jointTransform * totalLocalPos;
        totalLocalPos += posePosition * vertexWeights[i];

        vec4 worldNormal = jointTransform * totalNormal;
        totalNormal += worldNormal * vertexWeights[i];
   }
#endif

#if SWAY_MAPPING
	vec4 swayColour = texture(samplerSway, vertexUv);
	float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * (totalLocalPos.y / object.swayHeight) * length(totalLocalPos.xyz);
	totalLocalPos.x += swayPower * object.swayOffset.x;
	totalLocalPos.z += swayPower * object.swayOffset.y;
#endif

	vec4 worldPosition = object.transform * totalLocalPos;

    gl_Position = scene.projection * scene.view * worldPosition;

    fragmentUv = vertexUv;
	fragmentNormal = normalize((object.transform * totalNormal).xyz);

#if NORMAL_MAPPING
    mat3 normalMatrix = transpose(inverse(mat3(object.transform)));
    tangentT = normalize(normalMatrix * vertexTangent);
    tangentN = normalize(normalMatrix * vertexNormal);
    tangentB = normalize(cross(tangentT, tangentN));
#endif
}
