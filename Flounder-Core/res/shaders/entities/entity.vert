#version 450

#extension GL_ARB_separate_shader_objects : enable

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

layout(binding = 2) uniform sampler2D samplerSway;

layout(binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 clipPlane;
	mat4 modelMatrix;

	float atlasRows;
	vec2 atlasOffset;

	bool animated;
	mat4 jointTransforms[MAX_JOINTS];

	bool swaying;
	float swayHeight;
	vec2 swayOffset;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoords;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in ivec3 inJointIndices;
layout(location = 5) in vec3 inWeights;

layout(location = 0) out vec2 textureCoords;
layout(location = 1) out vec3 surfaceNormal;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

void main(void) 
{
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	if (ubo.animated) 
	{
		for (int i = 0; i < MAX_WEIGHTS; i++)
		{
			mat4 jointTransform = ubo.jointTransforms[inJointIndices[i]];
			vec4 posePosition = jointTransform * vec4(inPosition, 1.0);
			totalLocalPos += posePosition * inWeights[i];

			vec4 worldNormal = jointTransform * vec4(inNormal, 0.0);
			totalNormal += worldNormal * inWeights[i];
		}
	} 
	else 
	{
		totalLocalPos = vec4(inPosition, 1.0);
		totalNormal = vec4(inNormal, 0.0);
	}

	textureCoords = (inTextureCoords / ubo.atlasRows) + ubo.atlasOffset;

	if (ubo.swaying) 
	{
		vec4 swayColour = texture(samplerSway, inTextureCoords);
		float swayPower = 0.5 * exp(log(length(swayColour.rgb)) / 3.0) * (totalLocalPos.y / ubo.swayHeight) * length(totalLocalPos.xyz);
		totalLocalPos.x += swayPower * ubo.swayOffset.x;
		totalLocalPos.z += swayPower * ubo.swayOffset.y;
	}

	vec4 worldPosition = ubo.modelMatrix * totalLocalPos;

	gl_ClipDistance[0] = dot(worldPosition, ubo.clipPlane);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;

	surfaceNormal = normalize((ubo.modelMatrix * totalNormal).xyz);
}
