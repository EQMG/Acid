#pragma once

#include "Maths/Vector2.hpp"

namespace acid
{
enum class QuadraticSolution
{
	None, All, Touch, One, Two
};

class Rect
{
public:
	Vector2f m_min;
	Vector2f m_max;
};

static QuadraticSolution SolveQuadratic(const float &a, const float &b, const float &c, float &x1, float &x2);

static float LineVerticalIntersect(const float &x, const Vector2f &p1, const Vector2f &p2);

static float LineHorizontalIntersect(const float &y, const Vector2f &p1, const Vector2f &p2);

static bool IsBetween(const float &value, const float &min, const float &max);

static bool IsBetweenExclusive(const float &value, const float &min, const float &max);

static bool IsPointInsideBbox(const Rect &bbox, const Vector2f &p);

static bool IsPointInsideBboxExclusive(const Rect &bbox, const Vector2f &p);

static bool IsIntersectionInLineSegment(const Vector2f &p1, const Vector2f &p2, const Vector2f &i);

static bool IsLineSegmentIntersectingBbox(const Rect &bbox, const Vector2f &p1, const Vector2f &p2);

bool BboxBezier2Intersect(const Rect &bbox, const Vector2f bezier[3]);

float LineSignedDistance(const Vector2f &a, const Vector2f &b, const Vector2f &p);

float LineCalculateT(const Vector2f &a, const Vector2f &b, const Vector2f &p);

static void Bezier2Points(Vector2f &q0, Vector2f &q1, Vector2f &r, const Vector2f bezier[3], const float &t);

void Bezier2Point(Vector2f &r, const Vector2f bezier[3], const float &t);

void Bezier2SplitLr(Vector2f left[3], Vector2f right[3], const Vector2f bezier[3], const float &t);

void Bezier2Split_5P(Vector2f ret[5], const Vector2f bezier[3], const float &t);

void Bezier2Split_3P(Vector2f ret[3], const Vector2f bezier[3], const float &t);

void Bezier2Derivative(const Vector2f bezier[3], Vector2f derivative[2]);

static float Bezier2Component(const float &p0, const float &p1, const float &p2, const float &t);

void Bezier2Bbox(const Vector2f bezier[3], Rect &bbox);

static void AlignPoint(Vector2f &r, const Vector2f &p, const Vector2f &t, const float &s, const float &c);

static void AlignLsc(const Vector2f &p0, const Vector2f &p1, float &l, float &s, float &c);

void Bezier2AlignToSelf(Vector2f r[3], const Vector2f bezier[3]);

void Bezier2AlignToLine(Vector2f r[3], const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1);

bool Bezier2LineIsIntersecting(const Vector2f bezier[3], const Vector2f &line0, const Vector2f &line1);
}
