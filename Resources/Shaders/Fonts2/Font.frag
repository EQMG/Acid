#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define UDIST_BIAS 0.001f

layout (binding = 1) buffer CellBuffer
{
	uint cells[];
} cellBuffer;

layout (binding = 2) buffer PointBuffer
{
	vec2 points[];
} pointBuffer;

layout(location = 0) in vec2 inGlyphPos;
layout(location = 1) flat in uvec4 inCellInfo;
layout(location = 2) in float inSharpness;
layout(location = 3) in vec2 inCellCoord;
layout(location = 4) in vec4 inColour;

layout(location = 0) out vec4 outColour;

float calcT(vec2 a, vec2 b, vec2 p)
{
	vec2 dir = b - a;
	float t = dot(p - a, dir) / dot(dir, dir);
	return clamp(t, 0.0f, 1.0f);
}

float distToLine(vec2 a, vec2 b, vec2 p)
{
	vec2 dir = b - a;
	vec2 norm = vec2(-dir.y, dir.x);
	return dot(normalize(norm), a - p);
}

float distToBezier2(vec2 p0, vec2 p1, vec2 p2, float t, vec2 p)
{
	vec2 q0 = mix(p0, p1, t);
	vec2 q1 = mix(p1, p2, t);
	return distToLine(q0, q1, p);
}

void processBezier2(vec2 p, uint i, inout float minUdist, inout float v)
{
	vec2 p0 = pointBuffer.points[i];
	vec2 p1 = pointBuffer.points[i + 1];
	vec2 p2 = pointBuffer.points[i + 2];

	float t = calcT(p0, p2, p);
	float udist = distance(mix(p0, p2, t), p);

	if (udist <= minUdist + UDIST_BIAS)
	{
		float bez = distToBezier2(p0, p1, p2, t, p);

		if (udist >= minUdist - UDIST_BIAS)
		{
			vec2 prevp = pointBuffer.points[i - 2];
			float prevd = distToLine(p0, p2, prevp);
			v = mix(min(bez, v), max(bez, v), step(prevd, 0.0f));
		}
		else
		{
			v = bez;
		}

		minUdist = min(minUdist, udist);
	}
}

void processBezier2Loop(vec2 p, uint begin, uint end, inout float minUdist, inout float v)
{
	for (uint i = begin; i < end; i += 2)
	{
		processBezier2(p, i, minUdist, v);
	}
}

float cellSignedDist(uint pointOffset, uint cell, vec2 p)
{
	float minUdist = intBitsToFloat(2139095039);
	float v = -intBitsToFloat(2139095039);

	uvec3 vcell = uvec3(cell, cell, cell);
	uvec3 len = (vcell >> uvec3(0, 2, 5)) & uvec3(3, 7, 7);
	uvec3 begin = pointOffset + ((vcell >> uvec3(8, 16, 24)) & 0xFF) * 2;
	uvec3 end = begin + len * 2;

	processBezier2Loop(p, begin.x, end.x, minUdist, v);
	processBezier2Loop(p, begin.y, end.y, minUdist, v);
	processBezier2Loop(p, begin.z, end.z, minUdist, v);

	return v;
}

void main()
{
	uvec2 c = min(uvec2(inCellCoord), inCellInfo.zw - 1);
	uint cellIndex = inCellInfo.y + inCellInfo.z * c.y + c.x;
	uint cell = cellBuffer.cells[cellIndex];

	float v = cellSignedDist(inCellInfo.x, cell, inGlyphPos);
	float alpha = clamp(v * inSharpness + 0.5f, 0.0f, 1.0f);
	outColour = vec4(inColour.rgb, inColour.a * alpha);
}
