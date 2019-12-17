#pragma once

#include "Maths/Vector2.hpp"

namespace acid {
class ACID_EXPORT Rect {
public:
	enum class QuadraticSolution {
		None,
		All,
		Touch,
		One,
		Two
	};
	
	Rect() = default;
	Rect(const Vector2f &min, const Vector2f &max);
	Rect(const Vector2f bezier[3]);
	
	bool IsPointInsideBbox(const Vector2f &p) const;
	bool IsPointInsideBboxExclusive(const Vector2f &p) const;
	bool IsLineSegmentIntersectingBbox(const Vector2f &p1, const Vector2f &p2) const;
	bool BboxBezier2Intersect(const Vector2f bezier[3]) const;

	static QuadraticSolution SolveQuadratic(float a, float b, float c, float &x1, float &x2);
	static float LineVerticalIntersect(float x, const Vector2f &p1, const Vector2f &p2);
	static float LineHorizontalIntersect(float y, const Vector2f &p1, const Vector2f &p2);
	static bool IsBetween(float value, float min, float max);
	static bool IsBetweenExclusive(float value, float min, float max);
	static bool IsIntersectionInLineSegment(const Vector2f &p1, const Vector2f &p2, const Vector2f &i);
	static float LineSignedDistance(const Vector2f &a, const Vector2f &b, const Vector2f &p);
	static float LineCalculateT(const Vector2f &a, const Vector2f &b, const Vector2f &p);
	static void Bezier2Points(Vector2f &q0, Vector2f &q1, Vector2f &r, const Vector2f bezier[3], float t);
	static void Bezier2Point(Vector2f &r, const Vector2f bezier[3], float t);
	static void Bezier2SplitLr(Vector2f left[3], Vector2f right[3], const Vector2f bezier[3], float t);
	static void Bezier2Split5P(Vector2f ret[5], const Vector2f bezier[3], float t);
	static void Bezier2Split3P(Vector2f ret[3], const Vector2f bezier[3], float t);
	static void Bezier2Derivative(const Vector2f bezier[3], Vector2f derivative[2]);
	static float Bezier2Component(float p0, float p1, float p2, float t);
	static void AlignPoint(Vector2f &r, const Vector2f &p, const Vector2f &t, float s, float c);
	static void AlignLsc(const Vector2f &p0, const Vector2f &p1, float &l, float &s, float &c);
	static void Bezier2AlignToSelf(Vector2f r[3], const Vector2f bezier[3]);
	static void Bezier2AlignToLine(Vector2f r[3], const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1);
	static bool Bezier2LineIsIntersecting(const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1);
	
public:
	Vector2f m_min;
	Vector2f m_max;
};
}
