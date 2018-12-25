#version 450
#extension GL_ARB_separate_shader_objects : enable

#define UDIST_BIAS 0.001

layout (set = 0, binding = 1) buffer CellBuffer
{
	uint cells[];
} cell_buffer;

layout (set = 0, binding = 2) buffer PointBuffer
{
	vec2 points[];
} point_buffer;

layout(location = 0) in vec2 in_glyph_pos;
layout(location = 1) flat in uvec4 in_cell_info;
layout(location = 2) in float in_sharpness;
layout(location = 3) in vec2 in_cell_coord;

layout(location = 0) out vec4 out_color;

float calc_t(vec2 a, vec2 b, vec2 p)
{
	vec2 dir = b - a;
	float t = dot(p - a, dir) / dot(dir, dir);
	return clamp(t, 0.0, 1.0);
}

float dist_to_line(vec2 a, vec2 b, vec2 p)
{
	vec2 dir = b - a;
	vec2 norm = vec2(-dir.y, dir.x);
	return dot(normalize(norm), a - p);
}

float dist_to_bezier2(vec2 p0, vec2 p1, vec2 p2, float t, vec2 p)
{
	vec2 q0 = mix(p0, p1, t);
	vec2 q1 = mix(p1, p2, t);
	return dist_to_line(q0, q1, p);
}
/*
void process_bezier2(vec2 p, uint i, inout float min_udist, inout float v)
{
	vec2 p0 = point_buffer.points[i];
	vec2 p1 = point_buffer.points[i + 1];
	vec2 p2 = point_buffer.points[i + 2];

	float t = calc_t(p0, p2, p);
	float udist = distance(mix(p0, p2, t), p);

	if (udist < min_udist + UDIST_BIAS)
	{
		float bez = dist_to_bezier2(p0, p1, p2, t, p);

		v = mix(min(bez, v), max(bez, v), p.x);
		v = mix(v, bez, step(udist, min_udist - UDIST_BIAS));

		min_udist = min(min_udist, udist);
	}

}*/

void process_bezier2(vec2 p, uint i, inout float min_udist, inout float v)
{
	vec2 p0 = point_buffer.points[i];
	vec2 p1 = point_buffer.points[i + 1];
	vec2 p2 = point_buffer.points[i + 2];

	float t = calc_t(p0, p2, p);
	float udist = distance(mix(p0, p2, t), p);

	if (udist <= min_udist + UDIST_BIAS)
	{
		float bez = dist_to_bezier2(p0, p1, p2, t, p);

		if (udist >= min_udist - UDIST_BIAS)
		{
			vec2 prevp = point_buffer.points[i - 2];
			float prevd = dist_to_line(p0, p2, prevp);
			v = mix(min(bez, v), max(bez, v), step(prevd, 0.0));
		}
		else
		{
			v = bez;
		}

		min_udist = min(min_udist, udist);
	}
}

void process_bezier2_loop(vec2 p, uint begin, uint end, inout float min_udist, inout float v)
{
	for (uint i = begin; i < end; i += 2)
		process_bezier2(p, i, min_udist, v);
}

float cell_signed_dist(uint point_offset, uint cell, vec2 p)
{
	float min_udist = intBitsToFloat(2139095039);
	float v = -intBitsToFloat(2139095039);
/*
	uint len0 = cell & 3;
	uint len1 = (cell >> 2) & 7;
	uint len2 = (cell >> 5) & 7;

	uint begin0 = point_offset + ((cell >> 8) & 0xFF) * 2;
	uint begin1 = point_offset + ((cell >> 16) & 0xFF) * 2;
	uint begin2 = point_offset + ((cell >> 24) & 0xFF) * 2;

	uint end0 = begin0 + len0 * 2;
	uint end1 = begin1 + len1 * 2;
	uint end2 = begin2 + len2 * 2;

	process_bezier2_loop(p, begin0, end0, min_udist, v);
	process_bezier2_loop(p, begin1, end1, min_udist, v);
	process_bezier2_loop(p, begin2, end2, min_udist, v);
*/
	uvec3 vcell = uvec3(cell, cell, cell);
	uvec3 len = (vcell >> uvec3(0, 2, 5)) & uvec3(3, 7, 7);
	uvec3 begin = point_offset + ((vcell >> uvec3(8, 16, 24)) & 0xFF) * 2;
	uvec3 end = begin + len * 2;

	process_bezier2_loop(p, begin.x, end.x, min_udist, v);
	process_bezier2_loop(p, begin.y, end.y, min_udist, v);
	process_bezier2_loop(p, begin.z, end.z, min_udist, v);

	return v;
}

void main()
{
	uvec2 c = min(uvec2(in_cell_coord), in_cell_info.zw - 1);
	uint cell_index = in_cell_info.y + in_cell_info.z * c.y + c.x;
	uint cell = cell_buffer.cells[cell_index];

	float v = cell_signed_dist(in_cell_info.x, cell, in_glyph_pos);
	float alpha = clamp(v * in_sharpness + 0.5, 0.0, 1.0);
	out_color = vec4(0.0, 0.0, 0.0, alpha);

	//if ((c.x + c.y) % 2 == 0)
	//	out_color = mix(out_color, vec4(1.0, 0.0, 0.0, 1.0), 0.1);
}
