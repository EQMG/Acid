#version 130

//---------CONSTANT------------
const float PI = 3.1415926535897932384626433832795;

//---------IN------------
layout(location = 0) in vec3 in_position;

//---------UNIFORM------------
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 clipPlane;
uniform mat4 modelMatrix;
uniform vec3 waterOffset;

uniform float waveTime;
uniform float waveLength;
uniform float amplitude;
uniform float squareSize;
uniform float waterHeight;

//---------OUT------------
out vec3 pass_surfaceNormal;
out vec4 pass_clipSpace;

//---------DECODE------------
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

//---------HEIGHT OFFSET------------
float generateOffset(float x, float z, float val1, float val2)
{
	float radiansX = ((mod(x + z * x * val1, waveLength) / waveLength) + waveTime) * 2.0 * PI;
	float radiansZ = ((mod(val2 * (z * x + x * z), waveLength) / waveLength) + waveTime * 2.0) * 2.0 * PI;
	return amplitude * 0.5 * (sin(radiansZ) + sin(radiansX));
}

vec4 generateVertexOffset(float x, float z)
{
	return vec4(generateOffset(x, z, 0.2, 0.1), generateOffset(x, z, 0.1, 0.3), generateOffset(x, z, 0.15, 0.2), 0.0);
}

//---------MAIN------------
void main(void) 
{
	vec4 offsets = decode(in_position.z) * squareSize;

	vec4 thisVertex = modelMatrix * vec4(in_position.x + waterOffset.x, 0.0, in_position.y + waterOffset.z, 1.0);
	vec4 otherVertex1 = modelMatrix * vec4(in_position.x + waterOffset.x + offsets.z, 0.0, in_position.y + waterOffset.z + offsets.w, 1.0);
	vec4 otherVertex2 = modelMatrix * vec4(in_position.x + waterOffset.x + offsets.x, 0.0, in_position.y + waterOffset.z + offsets.y, 1.0);

	thisVertex += generateVertexOffset(thisVertex.x, thisVertex.z);
	otherVertex1 += generateVertexOffset(otherVertex1.x, otherVertex1.z);
	otherVertex2 += generateVertexOffset(otherVertex2.x, otherVertex2.z);

    vec4 worldPosition = thisVertex;

	gl_ClipDistance[0] = dot(thisVertex, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	vec3 tangent = otherVertex1.xyz - thisVertex.xyz;
    vec3 bitangent = otherVertex2.xyz - thisVertex.xyz;
    vec3 normal = -cross(tangent, bitangent);

	pass_surfaceNormal = normalize(normal);
    pass_clipSpace = projectionMatrix * viewMatrix * worldPosition;
}
