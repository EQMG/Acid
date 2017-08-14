#version 450

#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.1415926535897932384626433832795;

layout(binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 clipPlane;
	mat4 modelMatrix;
	vec3 waterOffset;

	float waveTime;
	float waveLength;
	float amplitude;
	float squareSize;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 surfaceNormal;
layout(location = 1) out vec4 clipSpace;

out gl_PerVertex 
{
	vec4 gl_Position;
	float gl_ClipDistance[];
};

vec4 decode(float n)
{
	float delta = mod(n, 3.0);
	float z2 = delta - 1.0;
	n = (n - delta) / 3.0;
	delta = mod(n, 3.0);
	float x2 = delta - 1.0;
	n = (n - delta) / 3.0;
	delta = mod(n, 3.0);
	float z1 = delta - 1.0;
	n = (n - delta) / 3.0;
	float x1 = n - 1.0;
	return vec4(x1, z1, x2, z2);
}

float generateOffset(float x, float z, float val1, float val2)
{
	float radiansX = 2.0 * PI * ((mod(x + z * x * val1, ubo.waveLength) / ubo.waveLength) + ubo.waveTime);
	float radiansZ = 2.0 * PI * ((mod(val2 * (z * x + x * z), ubo.waveLength) / ubo.waveLength) + ubo.waveTime * 2.0);
	return 0.5 * ubo.amplitude * (cos(radiansZ + sin(x)) + sin(radiansX - cos(z)));
}

vec4 generateVertexOffset(float x, float z)
{
	return vec4(generateOffset(x, z, 0.2, 0.1), generateOffset(x, z, 0.1, 0.3), generateOffset(x, z, 0.15, 0.2), 0.0);
}

void main(void) 
{
	vec4 offsets = decode(inPosition.z) * ubo.squareSize;

	vec4 thisVertex = ubo.modelMatrix * vec4(inPosition.x + ubo.waterOffset.x, 0.0, inPosition.y + ubo.waterOffset.z, 1.0);
	vec4 otherVertex1 = ubo.modelMatrix * vec4(inPosition.x + ubo.waterOffset.x + offsets.z, 0.0, inPosition.y + ubo.waterOffset.z + offsets.w, 1.0);
	vec4 otherVertex2 = ubo.modelMatrix * vec4(inPosition.x + ubo.waterOffset.x + offsets.x, 0.0, inPosition.y + ubo.waterOffset.z + offsets.y, 1.0);

	thisVertex += generateVertexOffset(thisVertex.x, thisVertex.z);
	otherVertex1 += generateVertexOffset(otherVertex1.x, otherVertex1.z);
	otherVertex2 += generateVertexOffset(otherVertex2.x, otherVertex2.z);

	vec4 worldPosition = thisVertex;

	gl_ClipDistance[0] = dot(thisVertex, ubo.clipPlane);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;

	vec3 tangent = otherVertex1.xyz - thisVertex.xyz;
	vec3 bitangent = otherVertex2.xyz - thisVertex.xyz;
	vec3 normal = -cross(tangent, bitangent);

	surfaceNormal = normalize(normal);
	clipSpace = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;
}
