#include "Geometry.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
QuadraticSolution SolveQuadratic(const float &a, const float &b, const float &c, float &x1, float &x2)
{
	auto discriminant = b * b - 4.0f * a * c;

	if (discriminant > 0.0f)
	{
		auto sqrtD = std::sqrt(discriminant);
		auto common = b >= 0.0f ? -b - sqrtD : -b + sqrtD;

		x1 = 2.0f * c / common;

		if (a == 0.0f)
		{
			return QuadraticSolution::One;
		}

		x2 = common / (2.0f * a);
		return QuadraticSolution::Two;
	}

	if (discriminant == 0.0f)
	{
		if (b == 0.0f)
		{
			if (a == 0.0f)
			{
				if (c == 0.0f)
				{
					return QuadraticSolution::All;
				}

				return QuadraticSolution::None;
			}

			x1 = 0.0f;
			return QuadraticSolution::Touch;
		}

		x1 = 2.0f * c / -b;
		return QuadraticSolution::Touch;
	}

	return QuadraticSolution::None;
}

float LineVerticalIntersect(const float &x, const Vector2f &p1, const Vector2f &p2)
{
	auto m = (p2.m_y - p1.m_y) / (p2.m_x - p1.m_x);
	return p1.m_y - m * (p1.m_x - x);
}

float LineHorizontalIntersect(const float &y, const Vector2f &p1, const Vector2f &p2)
{
	auto n = (p2.m_x - p1.m_x) / (p2.m_y - p1.m_y);
	return p1.m_x - n * (p1.m_y - y);
}

bool IsBetween(const float &value, const float &min, const float &max)
{
	return value >= min && value <= max;
}

bool IsBetweenExclusive(const float &value, const float &min, const float &max)
{
	return value > min && value < max;
}

bool IsPointInsideBbox(const Rect &bbox, const Vector2f &p)
{
	return IsBetween(p.m_x, bbox.m_min.m_x, bbox.m_max.m_x) && IsBetween(p.m_y, bbox.m_min.m_y, bbox.m_max.m_y);
}

bool IsPointInsideBboxExclusive(const Rect &bbox, const Vector2f &p)
{
	return IsBetweenExclusive(p.m_x, bbox.m_min.m_x, bbox.m_max.m_x) && IsBetweenExclusive(p.m_y, bbox.m_min.m_y, bbox.m_max.m_y);
}

bool IsIntersectionInLineSegment(const Vector2f &p1, const Vector2f &p2, const Vector2f &i)
{
	auto pxMin = std::min(p1.m_x, p2.m_x);
	auto pxMax = std::max(p1.m_x, p2.m_x);
	auto pyMin = std::min(p1.m_y, p2.m_y);
	auto pyMax = std::max(p1.m_y, p2.m_y);
	return IsBetween(i[0], pxMin, pxMax) && IsBetween(i[1], pyMin, pyMax);
}

bool IsLineSegmentIntersectingBbox(const Rect &bbox, const Vector2f &p1, const Vector2f &p2)
{
	if (IsPointInsideBboxExclusive(bbox, p1))
	{
		return true;
	}

	if (IsPointInsideBboxExclusive(bbox, p2))
	{
		return true;
	}

	auto xTop = LineHorizontalIntersect(bbox.m_max.m_y, p1, p2);
	auto xBottom = LineHorizontalIntersect(bbox.m_min.m_y, p1, p2);
	auto yLeft = LineVerticalIntersect(bbox.m_min.m_x, p1, p2);
	auto yRight = LineVerticalIntersect(bbox.m_max.m_x, p1, p2);

	Vector2f top{xTop, bbox.m_max.m_y};
	Vector2f bottom{xBottom, bbox.m_min.m_y};
	Vector2f left{bbox.m_min.m_x, yLeft};
	Vector2f right{bbox.m_max.m_x, yRight};

	if (IsBetween(xTop, bbox.m_min.m_x, bbox.m_max.m_x) && IsIntersectionInLineSegment(p1, p2, top))
	{
		return true;
	}

	if (IsBetween(xBottom, bbox.m_min.m_x, bbox.m_max.m_x) && IsIntersectionInLineSegment(p1, p2, bottom))
	{
		return true;
	}

	if (IsBetween(yLeft, bbox.m_min.m_y, bbox.m_max.m_y) && IsIntersectionInLineSegment(p1, p2, left))
	{
		return true;
	}

	if (IsBetween(yRight, bbox.m_min.m_y, bbox.m_max.m_y) && IsIntersectionInLineSegment(p1, p2, right))
	{
		return true;
	}

	return false;
}

bool BboxBezier2Intersect(const Rect &bbox, const Vector2f bezier[3])
{
	if (IsPointInsideBboxExclusive(bbox, bezier[0]))
	{
		return true;
	}

	if (IsPointInsideBboxExclusive(bbox, bezier[2]))
	{
		return true;
	}

	Vector2f bl{bbox.m_min.m_x, bbox.m_min.m_y};
	Vector2f br{bbox.m_max.m_x, bbox.m_min.m_y};
	Vector2f tl{bbox.m_min.m_x, bbox.m_max.m_y};
	Vector2f tr{bbox.m_max.m_x, bbox.m_max.m_y};

	return Bezier2LineIsIntersecting(bezier, bl, br) || Bezier2LineIsIntersecting(bezier, br, tr) || Bezier2LineIsIntersecting(bezier, tr, tl)
		|| Bezier2LineIsIntersecting(bezier, tl, bl);
}

float LineSignedDistance(const Vector2f &a, const Vector2f &b, const Vector2f &p)
{
	auto lineDir = b - a;
	assert(lineDir.Length() > 0.0f);

	Vector2f perpDir{-lineDir.m_y, lineDir.m_x};
	perpDir = perpDir.Normalize();

	auto dirToA = a - p;
	return perpDir.Dot(dirToA);
}

float LineCalculateT(const Vector2f &a, const Vector2f &b, const Vector2f &p)
{
	auto ab = b - a;
	auto ap = p - a;

	auto t = ap.Dot(ab) / ab.Dot(ab);
	return std::clamp(t, 0.0f, 1.0f);
}

void Bezier2Points(Vector2f &q0, Vector2f &q1, Vector2f &r, const Vector2f bezier[3], const float &t)
{
	q0 = bezier[0].Lerp(bezier[1], t);
	q1 = bezier[1].Lerp(bezier[2], t);
	r = q0.Lerp(q1, t);
}

void Bezier2Point(Vector2f &r, const Vector2f bezier[3], const float &t)
{
	Vector2f q0, q1;
	Bezier2Points(q0, q1, r, bezier, t);
}

void Bezier2SplitLr(Vector2f left[3], Vector2f right[3], const Vector2f bezier[3], const float &t)
{
	Vector2f q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	left[0] = bezier[0];
	left[1] = q0;
	left[2] = r;

	right[0] = r;
	right[1] = q1;
	right[2] = bezier[2];
}

void Bezier2Split5P(Vector2f ret[5], const Vector2f bezier[3], const float &t)
{
	Vector2f q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	ret[0] = bezier[0];
	ret[1] = q0;
	ret[2] = r;
	ret[3] = q1;
	ret[4] = bezier[2];
}

void Bezier2Split3P(Vector2f ret[3], const Vector2f bezier[3], const float &t)
{
	Vector2f q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	ret[0] = q0;
	ret[1] = r;
	ret[2] = q1;
}

void Bezier2Derivative(const Vector2f bezier[3], Vector2f derivative[2])
{
	derivative[0] = bezier[1] - bezier[0];
	derivative[0] *= 2.0f;

	derivative[1] = bezier[2] - bezier[1];
	derivative[1] *= 2.0f;
}

float Bezier2Component(const float &p0, const float &p1, const float &p2, const float &t)
{
	return Maths::Lerp(Maths::Lerp(p0, p1, t), Maths::Lerp(p1, p2, t), t);
}

void Bezier2Bbox(const Vector2f bezier[3], Rect &bbox)
{
	Vector2f deriv[2];
	Bezier2Derivative(bezier, deriv);

	auto tx = deriv[0].m_x / (deriv[0].m_x - deriv[1].m_x);
	auto ty = deriv[0].m_y / (deriv[0].m_y - deriv[1].m_y);

	bbox.m_min.m_x = std::min(bezier[0].m_x, bezier[2].m_x);
	bbox.m_min.m_y = std::min(bezier[0].m_y, bezier[2].m_y);
	bbox.m_max.m_x = std::max(bezier[0].m_x, bezier[2].m_x);
	bbox.m_max.m_y = std::max(bezier[0].m_y, bezier[2].m_y);

	if (0.0f <= tx && tx <= 1.0f)
	{
		auto x = Bezier2Component(bezier[0].m_x, bezier[1].m_x, bezier[2].m_x, tx);

		if (deriv[0].m_x < deriv[1].m_x)
		{
			bbox.m_min.m_x = std::min(bbox.m_min.m_x, x);
		}
		else
		{
			bbox.m_max.m_x = std::max(bbox.m_max.m_x, x);
		}
	}

	if (0.0f <= ty && ty <= 1.0f)
	{
		auto y = Bezier2Component(bezier[0].m_y, bezier[1].m_y, bezier[2].m_y, ty);

		if (deriv[0].m_y < deriv[1].m_y)
		{
			bbox.m_min.m_y = std::min(bbox.m_min.m_y, y);
		}
		else
		{
			bbox.m_max.m_y = std::max(bbox.m_max.m_y, y);
		}
	}
}

void AlignPoint(Vector2f &r, const Vector2f &p, const Vector2f &t, const float &s, const float &c)
{
	auto tmp = p - t;

	r.m_x = tmp.m_x * c - tmp.m_y * s;
	r.m_y = tmp.m_x * s + tmp.m_y * c;
}

void AlignLsc(const Vector2f &p0, const Vector2f &p1, float &l, float &s, float &c)
{
	auto v = p1 - p0;

	l = v.Length();
	s = -v.m_y / l;
	c = v.m_x / l;
}

void Bezier2AlignToSelf(Vector2f r[3], const Vector2f bezier[3])
{
	float l, s, c;
	AlignLsc(bezier[0], bezier[2], l, s, c);

	r[0] = Vector2f(0.0f, 0.0f);
	AlignPoint(r[1], bezier[1], bezier[0], s, c);
	r[2] = Vector2f(l, 0.0f);
}

void Bezier2AlignToLine(Vector2f r[3], const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1)
{
	float l, s, c;
	AlignLsc(line0, line1, l, s, c);

	AlignPoint(r[0], bezier[0], line0, s, c);
	AlignPoint(r[1], bezier[1], line0, s, c);
	AlignPoint(r[2], bezier[2], line0, s, c);
}

bool Bezier2LineIsIntersecting(const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1)
{
	float l, si, co;
	AlignLsc(line0, line1, l, si, co);

	Vector2f bez[3];
	AlignPoint(bez[0], bezier[0], line0, si, co);
	AlignPoint(bez[1], bezier[1], line0, si, co);
	AlignPoint(bez[2], bezier[2], line0, si, co);

	auto x0 = bez[0].m_x, y0 = bez[0].m_y;
	auto x1 = bez[1].m_x, y1 = bez[1].m_y;
	auto x2 = bez[2].m_x, y2 = bez[2].m_y;

	auto a = y0 - 2 * y1 + y2;
	auto b = 2 * (y1 - y0);
	auto c = y0;

	float t0, t1, xt0, xt1;
	auto sol = SolveQuadratic(a, b, c, t0, t1);

	switch (sol)
	{
	case QuadraticSolution::None:
	case QuadraticSolution::All:
		return false;
	case QuadraticSolution::Touch:
	case QuadraticSolution::One:
		xt0 = Bezier2Component(x0, x1, x2, t0);
		return IsBetween(t0, 0, 1) && IsBetween(xt0, 0, l);
	case QuadraticSolution::Two:
		xt0 = Bezier2Component(x0, x1, x2, t0);
		xt1 = Bezier2Component(x0, x1, x2, t1);
		return (IsBetween(t0, 0, 1) && IsBetween(xt0, 0, l)) || (IsBetween(t1, 0, 1) && IsBetween(xt1, 0, l));
	default:
		throw std::runtime_error("No quadratic solution could be found.");
	}
}
}
