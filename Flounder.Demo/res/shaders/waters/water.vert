#version 450

#extension GL_ARB_separate_shader_objects : enable

const float PI = 3.1415926535897932384626433832795;

layout(binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec4 clip;
} scene;

layout(binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 waterOffset;

	float waveTime;
	float waveLength;
	float amplitude;
	float squareSize;
	
	vec4 diffuseColour;

	float shineDamper;
	float reflectivity;

	float ignoreReflections;
} object;

layout(location = 0) in vec3 vertexPosition;

layout(location = 0) out vec3 fragmentNormal;
layout(location = 1) out vec4 fragmentClipSpace;

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
	float radiansX = 2.0 * PI * ((mod(x + z * x * val1, object.waveLength) / object.waveLength) + object.waveTime);
	float radiansZ = 2.0 * PI * ((mod(val2 * (z * x + x * z), object.waveLength) / object.waveLength) + object.waveTime * 2.0);
	return 0.5 * object.amplitude * (cos(radiansZ + sin(x)) + sin(radiansX - cos(z)));
}

vec4 generateVertexOffset(float x, float z)
{
	return vec4(generateOffset(x, z, 0.2, 0.1), generateOffset(x, z, 0.1, 0.3), generateOffset(x, z, 0.15, 0.2), 0.0);
}

void main(void) 
{
	vec4 offsets = decode(vertexPosition.z) * object.squareSize;

	vec4 thisVertex = object.transform * vec4(vertexPosition.x + object.waterOffset.x, 0.0, vertexPosition.y + object.waterOffset.z, 1.0);
	vec4 otherVertex1 = object.transform * vec4(vertexPosition.x + object.waterOffset.x + offsets.z, 0.0, vertexPosition.y + object.waterOffset.z + offsets.w, 1.0);
	vec4 otherVertex2 = object.transform * vec4(vertexPosition.x + object.waterOffset.x + offsets.x, 0.0, vertexPosition.y + object.waterOffset.z + offsets.y, 1.0);

	thisVertex += generateVertexOffset(thisVertex.x, thisVertex.z);
	otherVertex1 += generateVertexOffset(otherVertex1.x, otherVertex1.z);
	otherVertex2 += generateVertexOffset(otherVertex2.x, otherVertex2.z);

	vec4 worldPosition = thisVertex;

	gl_ClipDistance[0] = dot(thisVertex, scene.clip);
	gl_Position = scene.projection * scene.view * worldPosition;

	vec3 tangent = otherVertex1.xyz - thisVertex.xyz;
	vec3 bitangent = otherVertex2.xyz - thisVertex.xyz;
	vec3 normal = -cross(tangent, bitangent);

	fragmentNormal = normalize(normal);
	fragmentClipSpace = scene.projection * scene.view * worldPosition;
}
