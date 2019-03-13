#include "Geometry.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
QuadraticSolution SolveQuadratic(const float& a, const float& b, const float& c, float& x1, float& x2)
{
	float discriminant = b * b - 4.0f * a * c;

	if(discriminant > 0.0f)
		{
			float sqrt_d = std::sqrt(discriminant);
			float common = b >= 0.0f ? -b - sqrt_d : -b + sqrt_d;

			x1 = 2.0f * c / common;

			if(a == 0.0f)
				{
					return QuadraticSolution::One;
				}

			x2 = common / (2.0f * a);
			return QuadraticSolution::Two;
		}

	if(discriminant == 0.0f)
		{
			if(b == 0.0f)
				{
					if(a == 0.0f)
						{
							if(c == 0.0f)
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

float LineVerticalIntersect(const float& x, const Vector2& p1, const Vector2& p2)
{
	float m = (p2.m_y - p1.m_y) / (p2.m_x - p1.m_x);
	return p1.m_y - m * (p1.m_x - x);
}

float LineHorizontalIntersect(const float& y, const Vector2& p1, const Vector2& p2)
{
	float n = (p2.m_x - p1.m_x) / (p2.m_y - p1.m_y);
	return p1.m_x - n * (p1.m_y - y);
}

bool IsBetween(const float& value, const float& min, const float& max)
{
	return value >= min && value <= max;
}

bool IsBetweenExclusive(const float& value, const float& min, const float& max)
{
	return value > min && value < max;
}

bool IsPointInsideBbox(const Rect& bbox, const Vector2& p)
{
	return IsBetween(p.m_x, bbox.minX, bbox.maxX) && IsBetween(p.m_y, bbox.minY, bbox.maxY);
}

bool IsPointInsideBboxExclusive(const Rect& bbox, const Vector2& p)
{
	return IsBetweenExclusive(p.m_x, bbox.minX, bbox.maxX) && IsBetweenExclusive(p.m_y, bbox.minY, bbox.maxY);
}

bool IsIntersectionInLineSegment(const Vector2& p1, const Vector2& p2, const Vector2& i)
{
	float pxMin = std::min(p1.m_x, p2.m_x);
	float pxMax = std::max(p1.m_x, p2.m_x);
	float pyMin = std::min(p1.m_y, p2.m_y);
	float pyMax = std::max(p1.m_y, p2.m_y);
	return IsBetween(i[0], pxMin, pxMax) && IsBetween(i[1], pyMin, pyMax);
}

bool IsLineSegmentIntersectingBbox(const Rect& bbox, const Vector2& p1, const Vector2& p2)
{
	if(IsPointInsideBboxExclusive(bbox, p1))
		{
			return true;
		}

	if(IsPointInsideBboxExclusive(bbox, p2))
		{
			return true;
		}

	float xTop = LineHorizontalIntersect(bbox.maxY, p1, p2);
	float xBottom = LineHorizontalIntersect(bbox.minY, p1, p2);
	float yLeft = LineVerticalIntersect(bbox.minX, p1, p2);
	float yRight = LineVerticalIntersect(bbox.maxX, p1, p2);

	Vector2 top = Vector2(xTop, bbox.maxY);
	Vector2 bottom = Vector2(xBottom, bbox.minY);
	Vector2 left = Vector2(bbox.minX, yLeft);
	Vector2 right = Vector2(bbox.maxX, yRight);

	if(IsBetween(xTop, bbox.minX, bbox.maxX) && IsIntersectionInLineSegment(p1, p2, top))
		{
			return true;
		}

	if(IsBetween(xBottom, bbox.minX, bbox.maxX) && IsIntersectionInLineSegment(p1, p2, bottom))
		{
			return true;
		}

	if(IsBetween(yLeft, bbox.minY, bbox.maxY) && IsIntersectionInLineSegment(p1, p2, left))
		{
			return true;
		}

	if(IsBetween(yRight, bbox.minY, bbox.maxY) && IsIntersectionInLineSegment(p1, p2, right))
		{
			return true;
		}

	return false;
}

bool BboxBezier2Intersect(const Rect& bbox, const Vector2 bezier[3])
{
	if(IsPointInsideBboxExclusive(bbox, bezier[0]))
		{
			return true;
		}

	if(IsPointInsideBboxExclusive(bbox, bezier[2]))
		{
			return true;
		}

	Vector2 bl = Vector2(bbox.minX, bbox.minY);
	Vector2 br = Vector2(bbox.maxX, bbox.minY);
	Vector2 tl = Vector2(bbox.minX, bbox.maxY);
	Vector2 tr = Vector2(bbox.maxX, bbox.maxY);

	return Bezier2LineIsIntersecting(bezier, bl, br) || Bezier2LineIsIntersecting(bezier, br, tr) || Bezier2LineIsIntersecting(bezier, tr, tl) || Bezier2LineIsIntersecting(bezier, tl, bl);
}

float LineSignedDistance(const Vector2& a, const Vector2& b, const Vector2& p)
{
	Vector2 lineDir = b - a;
	assert(lineDir.Length() > 0.0f);

	Vector2 perpDir = Vector2(-lineDir.m_y, lineDir.m_x);
	perpDir = perpDir.Normalize();

	Vector2 dirToA = a - p;

	return perpDir.Dot(dirToA);
}

float LineCalculateT(const Vector2& a, const Vector2& b, const Vector2& p)
{
	Vector2 ab = b - a;
	Vector2 ap = p - a;

	float t = ap.Dot(ab) / ab.Dot(ab);
	return std::clamp(t, 0.0f, 1.0f);
}

void Bezier2Points(Vector2& q0, Vector2& q1, Vector2& r, const Vector2 bezier[3], const float& t)
{
	q0 = bezier[0].Lerp(bezier[1], t);
	q1 = bezier[1].Lerp(bezier[2], t);
	r = q0.Lerp(q1, t);
}

void Bezier2Point(Vector2& r, const Vector2 bezier[3], const float& t)
{
	Vector2 q0, q1;
	Bezier2Points(q0, q1, r, bezier, t);
}

void Bezier2SplitLr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], const float& t)
{
	Vector2 q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	left[0] = bezier[0];
	left[1] = q0;
	left[2] = r;

	right[0] = r;
	right[1] = q1;
	right[2] = bezier[2];
}

void Bezier2Split_5P(Vector2 ret[5], const Vector2 bezier[3], const float& t)
{
	Vector2 q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	ret[0] = bezier[0];
	ret[1] = q0;
	ret[2] = r;
	ret[3] = q1;
	ret[4] = bezier[2];
}

void Bezier2Split_3P(Vector2 ret[3], const Vector2 bezier[3], const float& t)
{
	Vector2 q0, q1, r;
	Bezier2Points(q0, q1, r, bezier, t);

	ret[0] = q0;
	ret[1] = r;
	ret[2] = q1;
}

void Bezier2Derivative(const Vector2 bezier[3], Vector2 derivative[2])
{
	derivative[0] = bezier[1] - bezier[0];
	derivative[0] *= 2.0f;

	derivative[1] = bezier[2] - bezier[1];
	derivative[1] *= 2.0f;
}

float Bezier2Component(const float& p0, const float& p1, const float& p2, const float& t)
{
	return Maths::Lerp(Maths::Lerp(p0, p1, t), Maths::Lerp(p1, p2, t), t);
}

void Bezier2Bbox(const Vector2 bezier[3], Rect& bbox)
{
	Vector2 deriv[2];
	Bezier2Derivative(bezier, deriv);

	float tx = deriv[0].m_x / (deriv[0].m_x - deriv[1].m_x);
	float ty = deriv[0].m_y / (deriv[0].m_y - deriv[1].m_y);

	bbox.minX = std::min(bezier[0].m_x, bezier[2].m_x);
	bbox.minY = std::min(bezier[0].m_y, bezier[2].m_y);
	bbox.maxX = std::max(bezier[0].m_x, bezier[2].m_x);
	bbox.maxY = std::max(bezier[0].m_y, bezier[2].m_y);

	if(0.0f <= tx && tx <= 1.0f)
		{
			float x = Bezier2Component(bezier[0].m_x, bezier[1].m_x, bezier[2].m_x, tx);

			if(deriv[0].m_x < deriv[1].m_x)
				{
					bbox.minX = std::min(bbox.minX, x);
				}
			else
				{
					bbox.maxX = std::max(bbox.maxX, x);
				}
		}

	if(0.0f <= ty && ty <= 1.0f)
		{
			float y = Bezier2Component(bezier[0].m_y, bezier[1].m_y, bezier[2].m_y, ty);

			if(deriv[0].m_y < deriv[1].m_y)
				{
					bbox.minY = std::min(bbox.minY, y);
				}
			else
				{
					bbox.maxY = std::max(bbox.maxY, y);
				}
		}
}

void AlignPoint(Vector2& r, const Vector2& p, const Vector2& t, const float& s, const float& c)
{
	Vector2 tmp = p - t;

	r.m_x = tmp.m_x * c - tmp.m_y * s;
	r.m_y = tmp.m_x * s + tmp.m_y * c;
}

void AlignLsc(const Vector2& p0, const Vector2& p1, float& l, float& s, float& c)
{
	Vector2 v = p1 - p0;

	l = v.Length();
	s = -v.m_y / l;
	c = v.m_x / l;
}

void Bezier2AlignToSelf(Vector2 r[3], const Vector2 bezier[3])
{
	float l, s, c;
	AlignLsc(bezier[0], bezier[2], l, s, c);

	r[0] = Vector2(0.0f, 0.0f);
	AlignPoint(r[1], bezier[1], bezier[0], s, c);
	r[2] = Vector2(l, 0.0f);
}

void Bezier2AlignToLine(Vector2 r[3], const Vector2 bezier[3], const Vector2& line0, const Vector2& line1)
{
	float l, s, c;
	AlignLsc(line0, line1, l, s, c);

	AlignPoint(r[0], bezier[0], line0, s, c);
	AlignPoint(r[1], bezier[1], line0, s, c);
	AlignPoint(r[2], bezier[2], line0, s, c);
}

bool Bezier2LineIsIntersecting(const Vector2 bezier[3], const Vector2& line0, const Vector2& line1)
{
	float l, si, co;
	AlignLsc(line0, line1, l, si, co);

	Vector2 bez[3];
	AlignPoint(bez[0], bezier[0], line0, si, co);
	AlignPoint(bez[1], bezier[1], line0, si, co);
	AlignPoint(bez[2], bezier[2], line0, si, co);

	float x0 = bez[0].m_x, y0 = bez[0].m_y;
	float x1 = bez[1].m_x, y1 = bez[1].m_y;
	float x2 = bez[2].m_x, y2 = bez[2].m_y;

	float a = y0 - 2 * y1 + y2;
	float b = 2 * (y1 - y0);
	float c = y0;

	float t0, t1, xt0, xt1;
	auto sol = SolveQuadratic(a, b, c, t0, t1);

	switch(sol)
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
				assert(false);
				return false;
		}
}
}
