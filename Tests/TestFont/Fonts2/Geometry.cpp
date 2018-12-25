#include "Geometry.hpp"

#include <assert.h>
#include "Maths/Maths.hpp"

namespace acid
{
	static QuadraticSolution solve_quadratic(const float &a, const float &b, const float &c, float &x1, float &x2)
	{
		float discriminant = b * b - 4.0f * a * c;

		if (discriminant > 0.0f)
		{
			float sqrt_d = std::sqrt(discriminant);
			float common = b >= 0.0f ? -b - sqrt_d : -b + sqrt_d;

			x1 = 2.0f * c / common;

			if (a == 0.0f)
			{
				return QUADRATIC_SOLUTION_ONE;
			}

			x2 = common / (2.0f * a);
			return QUADRATIC_SOLUTION_TWO;
		}
		else if (discriminant == 0.0f)
		{
			if (b == 0.0f)
			{
				if (a == 0.0f)
				{
					if (c == 0.0f)
					{
						return QUADRATIC_SOLUTION_ALL;
					}
					else
					{
						return QUADRATIC_SOLUTION_NONE;
					}
				}
				else
				{
					x1 = 0.0f;
					return QUADRATIC_SOLUTION_TOUCH;
				}
			}
			else
			{
				x1 = 2.0f * c / -b;
				return QUADRATIC_SOLUTION_TOUCH;
			}
		}

		return QUADRATIC_SOLUTION_NONE;
	}

	static float line_vertical_intersect(const float &x, const Vector2 &p1, const Vector2 &p2)
	{
		float m = (p2.m_y - p1.m_y) / (p2.m_x - p1.m_x);
		return p1.m_y - m * (p1.m_x - x);
	}

	static float line_horizontal_intersect(const float &y, const Vector2 &p1, const Vector2 &p2)
	{
		float n = (p2.m_x - p1.m_x) / (p2.m_y - p1.m_y);
		return p1.m_x - n * (p1.m_y - y);
	}

	static bool is_between(const float &value, const float &min, const float &max)
	{
		return value >= min && value <= max;
	}

	static bool is_between_exclusive(const float &value, const float &min, const float &max)
	{
		return value > min && value < max;
	}

	static bool is_point_inside_bbox_exclusive(const Rect &bbox, const Vector2 &p)
	{
		return is_between_exclusive(p.m_x, bbox.minX, bbox.maxX) && is_between_exclusive(p.m_y, bbox.minY, bbox.maxY);
	}

	static bool is_intersection_in_line_segment(const Vector2 &p1, const Vector2 &p2, const Vector2 &i)
	{
		float px_min = std::min(p1.m_x, p2.m_x);
		float px_max = std::max(p1.m_x, p2.m_x);
		float py_min = std::min(p1.m_y, p2.m_y);
		float py_max = std::max(p1.m_y, p2.m_y);
		return is_between(i.m_x, px_min, px_max) && is_between(i.m_y, py_min, py_max);
	}

	static bool is_line_segment_intersecting_bbox(const Rect &bbox, const Vector2 &p1, const Vector2 &p2)
	{
		if (is_point_inside_bbox_exclusive(bbox, p1))
		{
			return true;
		}

		if (is_point_inside_bbox_exclusive(bbox, p2))
		{
			return true;
		}

		float x_top = line_horizontal_intersect(bbox.maxY, p1, p2);
		float x_bottom = line_horizontal_intersect(bbox.minY, p1, p2);
		float y_left = line_vertical_intersect(bbox.minX, p1, p2);
		float y_right = line_vertical_intersect(bbox.maxX, p1, p2);

		Vector2 top = {x_top, bbox.maxY};
		Vector2 bottom = {x_bottom, bbox.minY};
		Vector2 left = {bbox.minX, y_left};
		Vector2 right = {bbox.maxX, y_right};

		if (is_between(x_top, bbox.minX, bbox.maxX) &&
		    is_intersection_in_line_segment(p1, p2, top))
		{
			return true;
		}

		if (is_between(x_bottom, bbox.minX, bbox.maxX) &&
		    is_intersection_in_line_segment(p1, p2, bottom))
		{
			return true;
		}

		if (is_between(y_left, bbox.minY, bbox.maxY) &&
		    is_intersection_in_line_segment(p1, p2, left))
		{
			return true;
		}

		return is_between(y_right, bbox.minY, bbox.maxY) &&
		       is_intersection_in_line_segment(p1, p2, right);
	}

	bool bbox_bezier2_intersect(const Rect &bbox, const Vector2 bezier[3])
	{
		if (is_point_inside_bbox_exclusive(bbox, bezier[0]))
		{
			return true;
		}

		if (is_point_inside_bbox_exclusive(bbox, bezier[2]))
		{
			return true;
		}

		Vector2 bl = {bbox.minX, bbox.minY};
		Vector2 br = {bbox.maxX, bbox.minY};
		Vector2 tl = {bbox.minX, bbox.maxY};
		Vector2 tr = {bbox.maxX, bbox.maxY};

		return bezier2_line_is_intersecting(bezier, bl, br) ||
		       bezier2_line_is_intersecting(bezier, br, tr) ||
		       bezier2_line_is_intersecting(bezier, tr, tl) ||
		       bezier2_line_is_intersecting(bezier, tl, bl);
	}

	float line_signed_distance(const Vector2 &a, const Vector2 &b, const Vector2 &p)
	{
		Vector2 line_dir = b - a;
		assert(line_dir.Length() > 0.0f);

		Vector2 perp_dir = {-line_dir.m_y, line_dir.m_x};
		perp_dir = perp_dir.Normalize();

		Vector2 dir_to_a = a - p;

		return perp_dir.Dot(dir_to_a);
	}

	float line_calculate_t(const Vector2 &a, const Vector2 &b, const Vector2 &p)
	{
		Vector2 ab = b - a;
		Vector2 ap = p - a;

		float t = ap.Dot(ab) / ab.Dot(ab);
		return std::clamp(t, 0.0f, 1.0f);
	}

	static void bezier2_points(Vector2 &q0, Vector2 &q1, Vector2 &r, const Vector2 bezier[3], float t)
	{
		q0 = bezier[0].Lerp(bezier[1], t);
		q1 = bezier[1].Lerp(bezier[2], t);
		r = q0.Lerp(q1, t);
	}

	void bezier2_point(Vector2 &r, const Vector2 bezier[3], const float &t)
	{
		Vector2 q0, q1;
		bezier2_points(q0, q1, r, bezier, t);
	}

	void bezier2_split_lr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], const float &t)
	{
		Vector2 q0, q1, r;
		bezier2_points(q0, q1, r, bezier, t);

		left[0] = bezier[0];
		left[1] = q0;
		left[2] = r;

		right[0] = r;
		right[1] = q1;
		right[2] = bezier[2];
	}

	void bezier2_split_5p(Vector2 ret[5], const Vector2 bezier[3], const float &t)
	{
		Vector2 q0, q1, r;
		bezier2_points(q0, q1, r, bezier, t);

		ret[0] = bezier[0];
		ret[1] = q0;
		ret[2] = r;
		ret[3] = q1;
		ret[4] = bezier[2];
	}

	void bezier2_split_3p(Vector2 ret[3], const Vector2 bezier[3], const float &t)
	{
		Vector2 q0, q1, r;
		bezier2_points(q0, q1, r, bezier, t);

		ret[0] = q0;
		ret[1] = r;
		ret[2] = q1;
	}

	void bezier2_derivative(const Vector2 bezier[3], Vector2 derivative[2])
	{
		derivative[0] = 2.0f * (bezier[1] - bezier[0]);
		derivative[1] = 2.0f * (bezier[2] - bezier[1]);
	}

	static float bezier2_component(const float &p0, const float &p1, const float &p2, const float &t)
	{
		return Maths::Lerp(Maths::Lerp(p0, p1, t), Maths::Lerp(p1, p2, t), t);
	}

	Rect bezier2_bbox(const Vector2 bezier[3])
	{
		Vector2 deriv[2];
		bezier2_derivative(bezier, deriv);

		float tx = deriv[0].m_x / (deriv[0].m_x - deriv[1].m_x);
		float ty = deriv[0].m_y / (deriv[0].m_y - deriv[1].m_y);

		Rect bbox = {
			std::min(bezier[0].m_x, bezier[2].m_x),
			std::min(bezier[0].m_y, bezier[2].m_y),
			std::max(bezier[0].m_x, bezier[2].m_x),
			std::max(bezier[0].m_y, bezier[2].m_y)
		};

		if (0.0f <= tx && tx <= 1.0f)
		{
			float x = bezier2_component(bezier[0].m_x, bezier[1].m_x, bezier[2].m_x, tx);

			if (deriv[0].m_x < deriv[1].m_x)
			{
				bbox.minX = std::min(bbox.minX, x);
			}
			else
			{
				bbox.maxX = std::max(bbox.maxX, x);
			}
		}

		if (0.0f <= ty && ty <= 1.0f)
		{
			float y = bezier2_component(bezier[0].m_y, bezier[1].m_y, bezier[2].m_y, ty);

			if (deriv[0].m_y < deriv[1].m_y)
			{
				bbox.minY = std::min(bbox.minY, y);
			}
			else
			{
				bbox.maxY = std::max(bbox.maxY, y);
			}
		}

		return bbox;
	}

	static Vector2 align_point(const Vector2 &p, const Vector2 &t, const float &s, const float &c)
	{
		Vector2 tmp = p - t;
		return Vector2(
			tmp.m_x * c - tmp.m_y * s,
			tmp.m_x * s + tmp.m_y * c
		);
	}

	static void align_lsc(const Vector2 &p0, const Vector2 &p1, float &l, float &s, float &c)
	{
		Vector2 v = p1 - p0;

		l = v.Length();
		s = -v.m_y / l;
		c = v.m_x / l;
	}

	void bezier2_align_to_self(Vector2 r[3], const Vector2 bezier[3])
	{
		float l, s, c;
		align_lsc(bezier[0], bezier[2], l, s, c);

		r[0] = Vector2(0.0f, 0.0f);
		r[1] = align_point(bezier[1], bezier[0], s, c);
		r[2] = Vector2(l, 0.0f);
	}

	void bezier2_align_to_line(Vector2 r[3], const Vector2 bezier[3], const Vector2 &line0, const Vector2 &line1)
	{
		float l, s, c;
		align_lsc(line0, line1, l, s, c);

		r[0] = align_point(bezier[0], line0, s, c);
		r[1] = align_point(bezier[1], line0, s, c);
		r[2] = align_point(bezier[2], line0, s, c);
	}

	bool bezier2_line_is_intersecting(const Vector2 bezier[3], const Vector2 &line0, const Vector2 &line1)
	{
		float l, si, co;
		align_lsc(line0, line1, l, si, co);

		Vector2 bez[3];
		bez[0] = align_point(bezier[0], line0, si, co);
		bez[1] = align_point(bezier[1], line0, si, co);
		bez[2] = align_point(bezier[2], line0, si, co);

		float x0 = bez[0].m_x, y0 = bez[0].m_y;
		float x1 = bez[1].m_x, y1 = bez[1].m_y;
		float x2 = bez[2].m_x, y2 = bez[2].m_y;

		float a = y0 - 2.0f * y1 + y2;
		float b = 2.0f * (y1 - y0);
		float c = y0;

		float t0, t1, xt0, xt1;
		auto sol = solve_quadratic(a, b, c, t0, t1);

		switch (sol)
		{
			case QUADRATIC_SOLUTION_NONE:
			case QUADRATIC_SOLUTION_ALL:
				return false;
			case QUADRATIC_SOLUTION_TOUCH:
			case QUADRATIC_SOLUTION_ONE:
				xt0 = bezier2_component(x0, x1, x2, t0);
				return is_between(t0, 0.0f, 1.0f) && is_between(xt0, 0.0f, l);
			case QUADRATIC_SOLUTION_TWO:
				xt0 = bezier2_component(x0, x1, x2, t0);
				xt1 = bezier2_component(x0, x1, x2, t1);
				return (is_between(t0, 0.0f, 1.0f) && is_between(xt0, 0.0f, l)) ||
				       (is_between(t1, 0.0f, 1.0f) && is_between(xt1, 0.0f, l));
			default:
				assert(false);
				return false;
		}
	}
}
