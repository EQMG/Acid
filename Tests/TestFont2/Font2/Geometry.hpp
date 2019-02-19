#pragma once

#include <stdint.h>
#include <Maths/Maths.hpp>
#include <Maths/Vector2.hpp>

namespace acid
{
	struct Rect
	{
		float min_x;
		float min_y;
		float max_x;
		float max_y;
	};

	bool bbox_bezier2_intersect(const Rect *bbox, const Vector2 bezier[3]);

	float line_signed_distance(Vector2 a, Vector2 b, Vector2 p);

	float line_calculate_t(Vector2 a, Vector2 b, Vector2 p);

	void bezier2_point(Vector2 &r, const Vector2 bezier[3], float t);

	void bezier2_split_lr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], float t);

	void bezier2_split_5p(Vector2 ret[5], const Vector2 bezier[3], float t);

	void bezier2_split_3p(Vector2 ret[3], const Vector2 bezier[3], float t);

	void bezier2_derivative(const Vector2 bezier[3], Vector2 derivative[2]);

	void bezier2_bbox(const Vector2 bezier[3], Rect *bbox);

	void bezier2_align_to_self(Vector2 r[3], const Vector2 bezier[3]);

	void bezier2_align_to_line(Vector2 r[3], const Vector2 bezier[3], Vector2 line0, Vector2 line1);

	bool bezier2_line_is_intersecting(const Vector2 bezier[3], Vector2 line0, Vector2 line1);
}
