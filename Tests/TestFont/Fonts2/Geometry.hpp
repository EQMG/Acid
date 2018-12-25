#pragma once

#include <array>
#include "Maths/Vector2.hpp"

namespace acid
{
	struct Rect
	{
		float minX;
		float minY;
		float maxX;
		float maxY;
	};

	enum QuadraticSolution
	{
		QUADRATIC_SOLUTION_NONE,
		QUADRATIC_SOLUTION_ALL,
		QUADRATIC_SOLUTION_TOUCH,
		QUADRATIC_SOLUTION_ONE,
		QUADRATIC_SOLUTION_TWO,
	};

	using Bezier3 = std::array<Vector2, 3>;

	static QuadraticSolution solve_quadratic(const float &a, const float &b, const float &c, float &x1, float &x2);

	static float line_vertical_intersect(const float &x, const Vector2 &p1, const Vector2 &p2);

	static float line_horizontal_intersect(const float &y, const Vector2 &p1, const Vector2 &p2);

	static bool is_between(const float &value, const float &min, const float &max);

	static bool is_between_exclusive(const float &value, const float &min, const float &max);

	static bool is_point_inside_bbox_exclusive(const Rect &bbox, const Vector2 &p);

	static bool is_intersection_in_line_segment(const Vector2 &p1, const Vector2 &p2, const Vector2 &i);

	static bool is_line_segment_intersecting_bbox(const Rect &bbox, const Vector2 &p1, const Vector2 &p2);

	bool bbox_bezier2_intersect(const Rect &bbox, const Vector2 bezier[3]);

	float line_signed_distance(const Vector2 &a, const Vector2 &b, const Vector2 &p);

	float line_calculate_t(const Vector2 &a, const Vector2 &b, const Vector2 &p);

	static void bezier2_points(Vector2 &q0, Vector2 &q1, Vector2 &r, const Vector2 bezier[3], float t);

	void bezier2_point(Vector2 &r, const Vector2 bezier[3], const float &t);

	void bezier2_split_lr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], const float &t);

	void bezier2_split_5p(Vector2 ret[5], const Vector2 bezier[3], const float &t);

	void bezier2_split_3p(Vector2 ret[3], const Vector2 bezier[3], const float &t);

	void bezier2_derivative(const Vector2 bezier[3], Vector2 derivative[2]);

	static float bezier2_component(const float &p0, const float &p1, const float &p2, const float &t);

	Rect bezier2_bbox(const Vector2 bezier[3]);

	static Vector2 align_point(const Vector2 &p, const Vector2 &t, const float &s, const float &c);

	static void align_lsc(const Vector2 &p0, const Vector2 &p1, float &l, float &s, float &c);

	void bezier2_align_to_self(Vector2 r[3], const Vector2 bezier[3]);

	void bezier2_align_to_line(Vector2 r[3], const Vector2 bezier[3], const Vector2 &line0, const Vector2 &line1);

	bool bezier2_line_is_intersecting(const Vector2 bezier[3], const Vector2 &line0, const Vector2 &line1);
}
