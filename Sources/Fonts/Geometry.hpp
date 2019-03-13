#pragma once

#include "Maths/Vector2.hpp"

namespace acid
{
enum class QuadraticSolution
{
	None,
	All,
	Touch,
	One,
	Two
};

struct Rect
{
	float minX;
	float minY;
	float maxX;
	float maxY;
};

static QuadraticSolution SolveQuadratic(const float& a, const float& b, const float& c, float& x1, float& x2);

static float LineVerticalIntersect(const float& x, const Vector2& p1, const Vector2& p2);

static float LineHorizontalIntersect(const float& y, const Vector2& p1, const Vector2& p2);

static bool IsBetween(const float& value, const float& min, const float& max);

static bool IsBetweenExclusive(const float& value, const float& min, const float& max);

static bool IsPointInsideBbox(const Rect& bbox, const Vector2& p);

static bool IsPointInsideBboxExclusive(const Rect& bbox, const Vector2& p);

static bool IsIntersectionInLineSegment(const Vector2& p1, const Vector2& p2, const Vector2& i);

static bool IsLineSegmentIntersectingBbox(const Rect& bbox, const Vector2& p1, const Vector2& p2);

bool BboxBezier2Intersect(const Rect& bbox, const Vector2 bezier[3]);

float LineSignedDistance(const Vector2& a, const Vector2& b, const Vector2& p);

float LineCalculateT(const Vector2& a, const Vector2& b, const Vector2& p);

static void Bezier2Points(Vector2& q0, Vector2& q1, Vector2& r, const Vector2 bezier[3], const float& t);

void Bezier2Point(Vector2& r, const Vector2 bezier[3], const float& t);

void Bezier2SplitLr(Vector2 left[3], Vector2 right[3], const Vector2 bezier[3], const float& t);

void Bezier2Split_5P(Vector2 ret[5], const Vector2 bezier[3], const float& t);

void Bezier2Split_3P(Vector2 ret[3], const Vector2 bezier[3], const float& t);

void Bezier2Derivative(const Vector2 bezier[3], Vector2 derivative[2]);

static float Bezier2Component(const float& p0, const float& p1, const float& p2, const float& t);

void Bezier2Bbox(const Vector2 bezier[3], Rect& bbox);

static void AlignPoint(Vector2& r, const Vector2& p, const Vector2& t, const float& s, const float& c);

static void AlignLsc(const Vector2& p0, const Vector2& p1, float& l, float& s, float& c);

void Bezier2AlignToSelf(Vector2 r[3], const Vector2 bezier[3]);

void Bezier2AlignToLine(Vector2 r[3], const Vector2 bezier[3], const Vector2& line0, const Vector2& line1);

bool Bezier2LineIsIntersecting(const Vector2 bezier[3], const Vector2& line0, const Vector2& line1);
}
